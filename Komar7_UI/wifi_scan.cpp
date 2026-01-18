#include "wifi_scan.h"
#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include "state.h"

extern Adafruit_SSD1306 display;

/* ===== CONFIG ===== */
#define MAX_AP 20
#define VISIBLE_LINES 4
#define SCROLLBAR_X 124
#define SCROLLBAR_Y 16
#define SCROLLBAR_H 40

/* ===== DATA ===== */
String ssidList[MAX_AP];
int rssiList[MAX_AP];
int apCount = 0;

static int scrollIndex = 0;

/* ===== SCAN STATE ===== */
enum ScanState {
  SCAN_IDLE,
  SCAN_RUNNING,
  SCAN_DONE
};

static ScanState scanState = SCAN_IDLE;
static unsigned long animTimer = 0;
static int animStep = 0;

/* ===== START SCAN ===== */
void wifiScanInit() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(100);

  WiFi.scanNetworks(true, true);   // async scan
  scanState = SCAN_RUNNING;
  animTimer = millis();
  animStep = 0;
}

/* ===== RESET ===== */
void wifiScanReset() {
  scanState = SCAN_IDLE;
  scrollIndex = 0;
}

/* ===== DRAW LOADING ===== */
void drawScanLoading() {
  display.clearDisplay();
  display.setCursor(20, 18);
  display.println("Scanning WiFi");

  // animasi titik ...
  display.setCursor(48, 32);
  for (int i = 0; i < animStep; i++) {
    display.print(".");
  }

  // progress bar
  int barWidth = (animStep * 100) / 4;
  display.drawRect(14, 48, 100, 6, SSD1306_WHITE);
  display.fillRect(14, 48, barWidth, 6, SSD1306_WHITE);

  display.display();
}

/* ===== DRAW SCROLL BAR ===== */
void drawScrollBar() {
  if (apCount <= VISIBLE_LINES) return;

  int barHeight = (SCROLLBAR_H * VISIBLE_LINES) / apCount;
  int barY = SCROLLBAR_Y +
             (SCROLLBAR_H * scrollIndex) / apCount;

  display.drawRect(SCROLLBAR_X, SCROLLBAR_Y,
                   3, SCROLLBAR_H, SSD1306_WHITE);

  display.fillRect(SCROLLBAR_X,
                   barY,
                   3,
                   barHeight,
                   SSD1306_WHITE);
}

/* ===== DRAW WIFI SCAN ===== */
void drawWifiScan() {

  /* ===== IDLE → START SCAN ===== */
  if (scanState == SCAN_IDLE) {
    wifiScanInit();
    return;
  }

  /* ===== RUNNING (LOADING) ===== */
  if (scanState == SCAN_RUNNING) {

    if (millis() - animTimer > 300) {
      animTimer = millis();
      animStep = (animStep + 1) % 5;
    }

    drawScanLoading();

    int result = WiFi.scanComplete();
    if (result == WIFI_SCAN_RUNNING) return;

    if (result >= 0) {
      apCount = result;
      if (apCount > MAX_AP) apCount = MAX_AP;

      for (int i = 0; i < apCount; i++) {
        ssidList[i] = WiFi.SSID(i);
        rssiList[i] = WiFi.RSSI(i);
      }

      WiFi.scanDelete();
      scrollIndex = 0;
      scanState = SCAN_DONE;
    }
    return;
  }

  /* ===== SCAN DONE ===== */
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("-- WiFi Scan --");
  display.println("----------------");

  for (int i = 0; i < VISIBLE_LINES; i++) {
    int idx = scrollIndex + i;
    if (idx >= apCount) break;

    display.print(idx + 1);
    display.print(". ");
    display.print(ssidList[idx]);
    display.print(" ");
    display.print(rssiList[idx]);
    display.println("dB");
  }

  drawScrollBar();
  display.display();
}

/* ===== SCROLL CONTROL ===== */
void wifiScanScrollUp() {
  if (scanState == SCAN_DONE && scrollIndex > 0)
    scrollIndex--;
}

void wifiScanScrollDown() {
  if (scanState == SCAN_DONE &&
      scrollIndex + VISIBLE_LINES < apCount)
    scrollIndex++;
}
