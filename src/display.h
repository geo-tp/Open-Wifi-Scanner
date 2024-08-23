#ifndef DISPLAY_H
#define DISPLAY_H

#include <algorithm>
#include <M5Cardputer.h> 
#include <wifi.h>

#define RECT_COLOR_DARK 0x0842
#define BACKGROUND_COLOR 0x0000

void displayInit();
void displayWelcome();
void displayLoading();
void displaySetBrightness(uint8_t brightness);
void displayTopBar(bool state);
void displayVolume(uint8_t volume);
void displayBottomBar();
void displayList(const std::vector<WiFiNetwork>& list);
void displayWifiCount(std::vector<WiFiNetwork> networks);
void drawWifiLogo(int centerX, int centerY, int radius, uint16_t iconColor);
void fillArc(int x, int y, int r1, int r2, int startAngle, int endAngle, uint16_t color);

#endif
