
#include "display.h"

void displayInit() {
    // Screen configuration
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setRotation(1);
    M5.Lcd.fillScreen(BACKGROUND_COLOR);
}

void displaySetBrightness(uint8_t brightness) {
  M5.Lcd.setBrightness(brightness);
}

void displayWelcome() {
    M5.Lcd.fillScreen(BACKGROUND_COLOR);

    // Box title
    M5.Lcd.fillRoundRect(31, 35, 175, 65, 30, RECT_COLOR_DARK);

    // Frame title
    M5.Lcd.drawRoundRect(30, 34, 177, 67, 30, TFT_DARKGRAY);

    // Main title
    M5.Lcd.setTextSize(1.65);
    M5.Lcd.setTextColor(TFT_LIGHTGREY);
    M5.Lcd.setCursor(103, 50);
    M5.Lcd.printf("OPEN WiFi");
    M5.Lcd.setCursor(101, 67);
    M5.Lcd.setTextSize(2.1);
    M5.Lcd.printf("SCANNER");

    // Logo
    drawWifiLogo(68, 68, 20, TFT_LIGHTGRAY);
};

void displayLoading() {
    M5.Lcd.fillRect(0, 32, 153, 75, BACKGROUND_COLOR);
    M5.Lcd.fillRoundRect(48, 58, 60, 20, 5, RECT_COLOR_DARK);
    M5.Lcd.setCursor(58, 64);
    M5.Lcd.setTextColor(TFT_DARKGREY);
    M5.Lcd.printf("Loading");

}

void displayList(const std::vector<WiFiNetwork>& list) {
    const uint16_t startX = 5;    // Position X
    uint16_t startY = 38;          // Position Y
    const uint16_t lineHeight = 17; // Hauteur
    const uint8_t maxItems = 4;    // Nombre max de la liste

    // Effacer l'écran
    M5.Lcd.fillRect(startX, startY-5, 148, 75, BACKGROUND_COLOR);
    
    M5.Lcd.setTextSize(1.2);

    if (list.empty()) {
        M5.Lcd.fillRoundRect(startX + 43, startY + 20, 60, 20, 5, RECT_COLOR_DARK);
        M5.Lcd.setCursor(startX+50, startY+25);
        M5.Lcd.setTextColor(TFT_DARKGREY);
        M5.Lcd.printf("No WiFi");
        return;
    }

    // Afficher 4 reseaux
    for (size_t i = 0; i < list.size() && i < maxItems; i++) {
        const WiFiNetwork& network = list[i];
        M5.Lcd.setCursor(startX, startY +  (i * lineHeight));
        
        if (network.webAccess) {
            M5.Lcd.setTextColor(TFT_GREEN);
            M5.Lcd.printf("v ");
            M5.Lcd.printf("%.15s", network.ssid.c_str());
        } else if (network.open) {
            M5.Lcd.setTextColor(TFT_GREEN);
            M5.Lcd.printf("- ");
            M5.Lcd.setTextColor(TFT_LIGHTGREY);
            M5.Lcd.printf("%.15s", network.ssid.c_str());
        } else if (network.vulnerable) {
            M5.Lcd.setTextColor(TFT_ORANGE);
            M5.Lcd.printf("x ");
            M5.Lcd.setTextColor(TFT_LIGHTGREY);
            M5.Lcd.printf("%.15s", network.ssid.c_str());
        } else {
            M5.Lcd.setTextColor(TFT_RED); 
            M5.Lcd.printf("x ");
            M5.Lcd.setTextColor(TFT_DARKGREY);
            M5.Lcd.printf("%.15s", network.ssid.c_str());
            M5.Lcd.setTextColor(TFT_LIGHTGREY);
        }
        
        M5.Lcd.fillRoundRect(startX + 112, startY - 2  + (i * lineHeight), 29, 13, 5, RECT_COLOR_DARK);
        M5.Lcd.setTextSize(1);
        uint16_t offset = encryptionTypeToString(network.encryption).size() < 4 ? 3 : 0;
        M5.Lcd.setCursor(startX + offset + 116, startY + 1 + (i * lineHeight));
        M5.Lcd.printf("%.4s", encryptionTypeToString(network.encryption).c_str());
    }
}

void displayTopBar(bool state) {
  M5.Lcd.drawRoundRect(0, 0, 148, 30, 5, TFT_DARKGREY); // frame around status
  M5.Lcd.fillRoundRect(1, 1, 146, 28, 5, RECT_COLOR_DARK);

  M5.Lcd.setTextSize(1.18);
  M5.Lcd.setCursor(12, 10);
  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.printf("State : ");

  M5.Lcd.setTextColor(TFT_LIGHTGREY);
  if (state) {
    M5.Lcd.setCursor(80, 10);
    M5.Lcd.drawCircle(69, 13, 4, TFT_GREEN);
    M5.Lcd.printf("Scanning");

  } else {
    M5.Lcd.setCursor(84, 10);
    M5.Lcd.drawCircle(69, 13, 4, TFT_RED);
    M5.Lcd.printf("Stopped");
  }
}

void displayVolume(uint8_t volume) {
  // Volume
  M5.Lcd.setTextSize(1.18);
  M5.Lcd.drawRoundRect(152, 0, 88, 30, 5, TFT_DARKGREY); // frame around titlte
  M5.Lcd.fillRoundRect(153, 1, 86, 28, 5, RECT_COLOR_DARK); // frame around titlte
  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.setCursor(166, 11);
  M5.Lcd.printf("Vol : ");
  M5.Lcd.setTextColor(TFT_LIGHTGREY);
  M5.Lcd.printf("%d", volume);
}

void displayBottomBar() {
  M5.Lcd.setTextSize(1.18);
  M5.Lcd.drawRoundRect(0, 108, 240, 30, 5, TFT_DARKGREY); // frame around controls
  M5.Lcd.fillRoundRect(1, 109, 238, 28, 5, RECT_COLOR_DARK); // frame around controls
  M5.Lcd.setCursor(13, 118);
  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.printf("Keys : ");
  M5.Lcd.setTextColor(TFT_LIGHTGREY);
  M5.Lcd.printf("< >");

  int startX1 = 92;  // Position X
  int startY1 = 119;  // Position Y
  int startX2 = 108;
  int startY2 = 119;
  int height = 6;    // Hauteur V
  int width  = 9;   // Largeur V

  // Dessiner la lettre "V"
  M5.Lcd.drawLine(startX1, startY1 + height, startX1 + width / 2, startY1, TFT_LIGHTGREY); // Ligne gauche
  M5.Lcd.drawLine(startX1 + width / 2, startY1, startX1 + width, startY1 + height, TFT_LIGHTGREY); // Ligne droite
  // Dessiner la lettre "V" inversée
  M5.Lcd.drawLine(startX2, startY2, startX2 + width / 2, startY2 + height, TFT_LIGHTGREY); // Ligne gauche inversée
  M5.Lcd.drawLine(startX2 + width / 2, startY2 + height, startX2 + width, startY2, TFT_LIGHTGREY); // Ligne droite inversée

  M5.Lcd.setTextColor(TFT_DARKGREY);
  M5.Lcd.printf("       Scan/Stop : ");
  M5.Lcd.setTextColor(TFT_LIGHTGREY);
  M5.Lcd.printf("ok");
}

void displayWifiCount(std::vector<WiFiNetwork> networks) {
    uint8_t webAccessNetworkCount = 0;
    uint8_t openNetworkCount = 0;
    uint8_t vulnerableNetworkCount = 0;
    uint8_t closedNetworkCount = 0;

    // Count networks
    for (const auto& network : networks) {
        if (network.webAccess) {
            webAccessNetworkCount++;
        } else if (network.open) {
            openNetworkCount++;
        } else if (network.vulnerable) {
            vulnerableNetworkCount++;
        } else {
            closedNetworkCount++;
        }
    }

    // Limit
    uint8_t maxNetworks = 99;
    uint8_t maxOpenNetworks = 9;
    closedNetworkCount = std::min(maxNetworks, closedNetworkCount);
    openNetworkCount = std::min(openNetworkCount, maxOpenNetworks);
    vulnerableNetworkCount = std::min(vulnerableNetworkCount, maxOpenNetworks);

    M5.Lcd.drawRoundRect(153, 34, 86, 70, 5, TFT_DARKGREY);
    M5.Lcd.fillRoundRect(154, 35, 84, 68, 5, RECT_COLOR_DARK); // reset area

    M5.Lcd.setTextSize(1.8);
    M5.Lcd.setCursor(172, 42);
    M5.Lcd.setTextColor(TFT_GREEN);
    M5.Lcd.println(webAccessNetworkCount);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(168, 59);
    M5.Lcd.print("web");

    M5.Lcd.setTextSize(1.8);
    M5.Lcd.setCursor(211, 42);
    M5.Lcd.setTextColor(TFT_LIGHTGREY);
    M5.Lcd.println(openNetworkCount);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(204, 59);
    M5.Lcd.print("open");

    M5.Lcd.setTextColor(TFT_ORANGE);
    M5.Lcd.setTextSize(1.8);
    M5.Lcd.setCursor(172, 73);
    M5.Lcd.print(vulnerableNetworkCount);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(166, 89);
    M5.Lcd.print("weak");
    
    M5.Lcd.setTextColor(TFT_RED);
    M5.Lcd.setTextSize(1.8);
    M5.Lcd.setCursor(closedNetworkCount > 9 ? 205 : 211, 73);
    M5.Lcd.print(closedNetworkCount);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(200, 89);
    M5.Lcd.print("close");
}

void drawWifiLogo(int centerX, int centerY, int radius, uint16_t iconColor) { 
    int numArcs = 3;

    for (int i = 0; i < numArcs; ++i) {
        int arcRadius = radius - (i * (radius / numArcs));
        int arcThickness = radius / (numArcs * 3);

        M5.Lcd.fillArc(centerX, centerY, arcRadius - arcThickness, arcRadius, 135, 45, iconColor);
    }

    M5.Lcd.fillCircle(centerX, centerY, radius / 6, iconColor);
}

void fillArc(int x, int y, int r1, int r2, int startAngle, int endAngle, uint16_t color) {
    for (int r = r1; r <= r2; r++) {
        for (int angle = startAngle; angle >= endAngle; angle--) {
            int x1 = x + r * cos(angle * DEG_TO_RAD);
            int y1 = y - r * sin(angle * DEG_TO_RAD);
            M5.Lcd.fillRect(x1, y1, 1, 1, color);
        }
    }
}