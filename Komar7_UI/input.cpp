#include "input.h"
#include "state.h"
#include "wifi_scan.h"

SystemState currentState = STATE_MAIN_MENU;
int menuIndex = 0;

const char* mainMenu[] = {"Bluetooth", "Wifi"};
const char* btMenu[]   = {"Blu Jam", "Low Blu Jam"};
const char* wifiMenu[] = {"Wifi Scan", "Deauth"};

void setupInput() {
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
}

bool buttonPressed(uint8_t pin) {
  static uint32_t lastPress[40];
  if (digitalRead(pin) == LOW) {
    if (millis() - lastPress[pin] > 200) {
      lastPress[pin] = millis();
      return true;
    }
  }
  return false;
}

void handleNavigation() {
  /* baru*/
    if (buttonPressed(BTN_UP)) {
    if (currentState == STATE_WIFI_SCAN)
      wifiScanScrollUp();
    else
      menuIndex = (menuIndex == 0) ? 1 : 0;
  }

  if (buttonPressed(BTN_DOWN)) {
    if (currentState == STATE_WIFI_SCAN)
      wifiScanScrollDown();
    else
      menuIndex = (menuIndex == 1) ? 0 : 1;
  }

  /*lama */

  // if (buttonPressed(BTN_UP)) {
  //   menuIndex = (menuIndex == 0) ? 1 : 0;
  // }

  // if (buttonPressed(BTN_DOWN)) {
  //   menuIndex = (menuIndex == 1) ? 0 : 1;
  // }

  if (buttonPressed(BTN_SELECT)) {
    if (currentState == STATE_MAIN_MENU)
      currentState = (menuIndex == 0) ? STATE_BT_MENU : STATE_WIFI_MENU;
    else if (currentState == STATE_BT_MENU)
      currentState = (menuIndex == 0) ? STATE_BLU_JAM : STATE_LOW_BLU_JAM;
    else if (currentState == STATE_WIFI_MENU)
      currentState = (menuIndex == 0) ? STATE_WIFI_SCAN : STATE_DEAUTH;

    menuIndex = 0;
  }

  if (buttonPressed(BTN_BACK)) {
    if (currentState == STATE_BT_MENU || currentState == STATE_WIFI_MENU)
      currentState = STATE_MAIN_MENU;
    else if (currentState == STATE_BLU_JAM || currentState == STATE_LOW_BLU_JAM)
      currentState = STATE_BT_MENU;
    else if (currentState == STATE_WIFI_SCAN) {
      wifiScanReset();
    currentState = STATE_WIFI_MENU;
    }

    menuIndex = 0;
  }
}
