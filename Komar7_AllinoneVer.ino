#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ================= OLED ================= */
#define OLED_SDA 4
#define OLED_SCL 5
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* ================= BUTTON ================= */
#define BTN_SELECT 25
#define BTN_UP     26
#define BTN_DOWN   27
#define BTN_BACK   32

/* ================= STATE ================= */
enum SystemState {
  STATE_MAIN_MENU,
  STATE_BT_MENU,
  STATE_WIFI_MENU,
  STATE_BLU_JAM,
  STATE_LOW_BLU_JAM,
  STATE_WIFI_SCAN,
  STATE_DEAUTH
};

SystemState currentState = STATE_MAIN_MENU;
int menuIndex = 0;

/* ================= MENU DATA ================= */
const char* mainMenu[] = {"Bluetooth", "Wifi"};
const char* btMenu[]   = {"Blu Jam", "Low Blu Jam"};
const char* wifiMenu[] = {"Wifi Scan", "Deauth"};

/* ================= TIMER ================= */
unsigned long mainMenuStart;

/* ================= BOOT ================= */
void bootScreen() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(20, 18);
  display.println("Komar7");

  display.setTextSize(1);
  display.setCursor(38, 42);
  display.println("By Ruins");

  display.display();
  delay(2000);
}

/* ================= BUTTON ================= */
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

/* ================= DRAW MENU ================= */
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

/* ================= STATUS SCREEN ================= */
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

/* ================= MAIN MENU STATUS ================= */
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

/* ================= SETUP ================= */
void setup() {
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  bootScreen();
  mainMenuStart = millis();
}

/* ================= LOOP ================= */
void loop() {

  /* ===== NAVIGATION ===== */
  if (buttonPressed(BTN_UP)) {
    menuIndex = (menuIndex == 0) ? 1 : 0;
  }

  if (buttonPressed(BTN_DOWN)) {
    menuIndex = (menuIndex == 1) ? 0 : 1;
  }

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
    else if (currentState == STATE_WIFI_SCAN || currentState == STATE_DEAUTH)
      currentState = STATE_WIFI_MENU;

    menuIndex = 0;
  }

  /* ===== DRAW ===== */
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
      drawStatus("Wifi Scan");
      break;

    case STATE_DEAUTH:
      drawStatus("Deauth");
      break;
  }

  delay(50);
}
