
#include "wifi.h"

void wifiInit() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}

void wifiReset() {
    WiFi.disconnect(true); // full off
    delay(500);

    WiFi.mode(WIFI_STA);
    delay(500);
}

std::vector<WiFiNetwork> getWifiNetworks() {
    std::vector<WiFiNetwork> networks;

    int n = WiFi.scanNetworks(false, true);
    for (int i = 0; i < n; i++) {
        WiFiNetwork network;
        network.ssid = WiFi.SSID(i).c_str();
        if (network.ssid.empty()) {network.ssid = "Hidden SSID";};
        network.encryption = WiFi.encryptionType(i);

        networks.push_back(network);
    }

    return networks;
}

std::vector<WiFiNetwork> getOpenWifiNetworks(std::vector<WiFiNetwork>& networks) {
    std::vector<WiFiNetwork> openNetworks;
    
    for (auto& network : networks) {
        if (network.encryption == WIFI_AUTH_OPEN) {
            network.open = true;
            openNetworks.push_back(network);
        }
    }

    return openNetworks;
}

std::vector<WiFiNetwork> getClosedWifiNetworks(const std::vector<WiFiNetwork>& networks) {
    std::vector<WiFiNetwork> closedNetworks;

    for (const auto& network : networks) {
        if (network.encryption != WIFI_AUTH_OPEN) {
            closedNetworks.push_back(network);
        }
    }

    return closedNetworks;
}

std::vector<WiFiNetwork> getValidatedOpenWifiNetworks(std::vector<WiFiNetwork>& networks) {
    std::vector<WiFiNetwork> validatedNetworks;
    
    for (auto& network : networks) {
        if (network.encryption == WIFI_AUTH_OPEN) {
            if (testOpenWifiConnection(network)) {
                network.webAccess = true;
                validatedNetworks.push_back(network);
            }
        }
    }

    return validatedNetworks;
}

std::vector<WiFiNetwork> getVulnerableWifiNetworks(std::vector<WiFiNetwork>& networks) {
    std::vector<WiFiNetwork> vulnerableNetworks;

    for (auto& network : networks) {
        if (network.encryption == WIFI_AUTH_WEP || 
            network.encryption == WIFI_AUTH_WPA_PSK) {
            network.vulnerable = true;
            vulnerableNetworks.push_back(network);
        }
    }

    return vulnerableNetworks;
}

bool testOpenWifiConnection(const WiFiNetwork& network) {
    WiFi.begin(network.ssid.c_str());

    int maxRetries = 3;
    while (WiFi.status() != WL_CONNECTED && maxRetries > 0) {
        delay(300);
        maxRetries--;
    }

    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }

    HTTPClient http;
    http.begin("http://example.com");
    int httpCode = http.GET();
    http.end();

    WiFi.disconnect();

    return httpCode > 0; // any http code confirm the web access
}

std::string encryptionTypeToString(wifi_auth_mode_t encryption) {
    switch (encryption) {
        case WIFI_AUTH_OPEN: return "OPEN";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA";
        case WIFI_AUTH_WPA2_PSK: return "WPA2";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA+";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2";
        case WIFI_AUTH_WPA3_PSK: return "WPA3";
        case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA*";
        case WIFI_AUTH_WAPI_PSK: return "WAPI";
        default: return "UNKNOWN";
    }
}

std::vector<WiFiNetwork> mergeAndOrderWifiNetworks(
    std::vector<WiFiNetwork>& closedNetworks,
    std::vector<WiFiNetwork>& openNetworks,
    std::vector<WiFiNetwork>& webAccessNetworks,
    std::vector<WiFiNetwork>& vulnerableNetworks) {
    
    std::vector<WiFiNetwork> mergedNetworks;

    // Add web access networks first
    mergedNetworks.insert(mergedNetworks.end(), webAccessNetworks.begin(), webAccessNetworks.end());

    // Remove web access networks from open networks
    openNetworks.erase(std::remove_if(openNetworks.begin(), openNetworks.end(),
        [&webAccessNetworks](const WiFiNetwork& network) {
            return std::any_of(webAccessNetworks.begin(), webAccessNetworks.end(),
                [&network](const WiFiNetwork& webNetwork) {
                    return webNetwork.ssid == network.ssid;
                });
        }), openNetworks.end());

    // then merge open networks
    mergedNetworks.insert(mergedNetworks.end(), openNetworks.begin(), openNetworks.end());

    // Add vulnerable networks
    mergedNetworks.insert(mergedNetworks.end(), vulnerableNetworks.begin(), vulnerableNetworks.end());

    // Remove vulnerables from closed networks
    closedNetworks.erase(std::remove_if(closedNetworks.begin(), closedNetworks.end(),
        [&vulnerableNetworks](const WiFiNetwork& network) {
            return std::any_of(vulnerableNetworks.begin(), vulnerableNetworks.end(),
                [&network](const WiFiNetwork& vulnNetwork) {
                    return vulnNetwork.ssid == network.ssid;
                });
        }), closedNetworks.end());

    // then merge closed networks
    mergedNetworks.insert(mergedNetworks.end(), closedNetworks.begin(), closedNetworks.end());

    // by default, nearest WiFi of each category will be first
    return mergedNetworks;
}