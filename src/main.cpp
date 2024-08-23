#include <M5Cardputer.h>
#include <FastLED.h>
#include <algorithm>
#include <thread>
#include <atomic>

#include <wifi.h>
#include <display.h>
#include <input.h>
#include <speaker.h>

#define PIN_LED    21 // Builtin
#define NUM_LEDS   1

CRGB leds[NUM_LEDS]; // init RGB LED builtin

// Mutex to avoid display conflict between threads
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// Netwoks holders
std::vector<WiFiNetwork> webAccessNetworks;
std::vector<WiFiNetwork> openNetworks;
std::vector<WiFiNetwork> closedNetworks;
std::vector<WiFiNetwork> vulnerableNetworks;
std::vector<WiFiNetwork> finalNetworks;

// Networks state
bool lastState = false;
bool webAccess = false;
bool openAuth = false;
bool vulnerable = false;

// App state
std::atomic<bool> running(true);
std::atomic<int16_t> volume(120);
std::atomic<int16_t> brightness(150);

// Scan timing
unsigned long startScan;
unsigned long endScan;
unsigned long scanDuration;

void handleInput(std::function<char()> handler) {
    int16_t minimum = 0;
    int16_t maximum = 255;
    uint8_t step = 20;

    while (true) {
        char input = handler();

        if (input != KEY_NONE) {
            portENTER_CRITICAL(&mux); // Thread lock to update screen
                switch (input) {
                    case KEY_OK:
                        running = !running;
                        displayTopBar(running);
                        break;
                    case KEY_ARROW_UP:
                        volume = std::min(static_cast<int16_t>(volume + step), maximum);
                        speakerSetVolume(volume);
                        displayVolume(volume);
                        break;
                    case KEY_ARROW_DOWN:
                        volume = std::max(static_cast<int16_t>(volume - step), minimum);
                        speakerSetVolume(volume);
                        displayVolume(volume);
                        break;
                    case KEY_ARROW_LEFT:
                        brightness = std::max(static_cast<int16_t>(brightness - step*2), minimum);
                        displaySetBrightness(brightness);
                        break;
                    case KEY_ARROW_RIGHT:
                        brightness = std::min(static_cast<int16_t>(brightness + step*2), maximum);
                        displaySetBrightness(brightness);
                        break;
                    default:
                        break;
                }
            portEXIT_CRITICAL(&mux); // release
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // wait 50ms
    }
}

void handleState() {
    webAccess = !webAccessNetworks.empty();
    vulnerable = !vulnerableNetworks.empty();
    openAuth = !openNetworks.empty();
    
    if (lastState != webAccess) {
        if (webAccess) {
            leds[0] = CRGB::Green;
            FastLED.show();
            speakerRingWebAccess();
            delay(3000);
        }
        lastState = webAccess;
    } 

    if (openAuth || webAccess) {
        leds[0] = CRGB::Green;
        FastLED.show();
        speakerRingOpen();
        delay(1000);
    } else if (vulnerable) {
        leds[0] = CRGB::Orange;
        FastLED.show();
        speakerRingVulnerable();
        delay(500);
    } else {
        leds[0] = CRGB::Red;
        FastLED.show();
    }
}

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg);

    // Init LED BUILTIN
    FastLED.addLeds<WS2812, PIN_LED, GRB>(leds, 1);
    leds[0] = CRGB::Red;
    FastLED.show();

    displayInit();
    speakerInit();
    wifiInit();

    displayWelcome();
    delay(3000);

    displayTopBar(running);
    displayVolume(volume);
    displayBottomBar();
    displayWifiCount({});
    displayLoading();

    // Start inputs thread
    std::thread inputThread(handleInput, cardputerInputHandler);
    inputThread.detach();
}

void loop() {
    if (running) {
        // Get available networks, it takes between 500-2000ms in general
        startScan = millis();
        std::vector<WiFiNetwork> allNetworks = getWifiNetworks();
        endScan = millis();
        scanDuration = endScan- startScan;

        // Could be already in the loop when bool running was toggled by the other thread
        if (!running) {
            return;
        }

        // Network scan could be very fast, in this case, WiFi could be locked
        // It seems to occur when WiFi loses connection during an HTTP request
        if (scanDuration < 20) {
            wifiReset();
            return;
        }

        // Get ordered networks
        closedNetworks = getClosedWifiNetworks(allNetworks);
        openNetworks = getOpenWifiNetworks(allNetworks);
        vulnerableNetworks = getVulnerableWifiNetworks(closedNetworks);
        webAccessNetworks = getValidatedOpenWifiNetworks(openNetworks);
        finalNetworks = mergeAndOrderWifiNetworks(closedNetworks, openNetworks, webAccessNetworks, vulnerableNetworks);

        // Thread lock to update screen
        portENTER_CRITICAL(&mux); 
            displayList(finalNetworks);
            displayWifiCount(finalNetworks);
        portEXIT_CRITICAL(&mux); // release

        handleState(); // update app state
    } else {
        delay(50);
    }
}