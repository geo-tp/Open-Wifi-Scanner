#ifndef WIFI_H
#define WIFI_H

#include <M5Cardputer.h>
#include <WiFi.h>
#include <HTTPClient.h>

struct WiFiNetwork {
    std::string ssid;
    wifi_auth_mode_t encryption;
    int signalStrength;
    bool open = false;
    bool webAccess = false;
    bool vulnerable = false;
};

void wifiInit();
void wifiReset();
std::vector<WiFiNetwork> getWifiNetworks();
std::vector<WiFiNetwork> getOpenWifiNetworks(std::vector<WiFiNetwork>& networks);
std::vector<WiFiNetwork> getClosedWifiNetworks(const std::vector<WiFiNetwork>& networks);
std::vector<WiFiNetwork> getVulnerableWifiNetworks(std::vector<WiFiNetwork>& networks);
std::vector<WiFiNetwork> getValidatedOpenWifiNetworks(std::vector<WiFiNetwork>& networks);
bool testOpenWifiConnection(const WiFiNetwork& network);
std::string encryptionTypeToString(wifi_auth_mode_t encryption);
std::vector<WiFiNetwork> mergeAndOrderWifiNetworks(
    std::vector<WiFiNetwork>& closedNetworks,
    std::vector<WiFiNetwork>& openNetworks,
    std::vector<WiFiNetwork>& webAccessNetworks,
    std::vector<WiFiNetwork>& vulnerableNetworks);

#endif
