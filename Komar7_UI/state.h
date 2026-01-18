#pragma once

enum SystemState {
  STATE_MAIN_MENU,
  STATE_BT_MENU,
  STATE_WIFI_MENU,
  STATE_BLU_JAM,
  STATE_LOW_BLU_JAM,
  STATE_WIFI_SCAN,
  STATE_DEAUTH
};

extern SystemState currentState;
extern int menuIndex;
extern unsigned long mainMenuStart;

/* Menu data */
extern const char* mainMenu[];
extern const char* btMenu[];
extern const char* wifiMenu[];
