#include "display_ui.h"
#include "state.h"
#include "wifi_scan.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long mainMenuStart = 0;

void setupDisplay() {
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
}

void bootScreen() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(20, 18);
  display.println("Komar7");

  display.setTextSize(1);
  display.setCursor(38, 42);
  display.println("By Ruins");

  display.display();
  delay(2000);
}

void drawMenu(const char* header, const char* items[], int count) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(header);
  display.println("----------------");

  for (int i = 0; i < count; i++) {
    display.print(i == menuIndex ? "> " : "  ");
    display.println(items[i]);
  }

  display.display();
}

void drawStatus(const char* title) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(title);
  display.println("----------------");
  display.println("Status : ACTIVE");
  display.println("Mode   : READY");
  display.println("");
  display.println("BACK to return");
  display.display();
}

void drawMainMenuStatus() {
  unsigned long uptime = (millis() - mainMenuStart) / 1000;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("---- Komar7 Menu ----");
  display.println("     ====   ====     ");

  for (int i = 0; i < 2; i++) {
    display.print(i == menuIndex ? "> " : "  ");
    display.println(mainMenu[i]);
  }

  display.println("");
  display.print("Uptime: ");
  display.print(uptime);
  display.println("s");

  display.display();
}

void drawCurrentState() {
  switch (currentState) {
    case STATE_MAIN_MENU:
      drawMainMenuStatus();
      break;
    case STATE_BT_MENU:
      drawMenu("-- Bluetooth --", btMenu, 2);
      break;
    case STATE_WIFI_MENU:
      drawMenu("-- Wifi --", wifiMenu, 2);
      break;
    case STATE_BLU_JAM:
      drawStatus("Blu Jam");
      break;
    case STATE_LOW_BLU_JAM:
      drawStatus("Low Blu Jam");
      break;
    case STATE_WIFI_SCAN:
      drawWifiScan();
      break;
    case STATE_DEAUTH:
      drawStatus("Deauth");
      break;
  }
}
