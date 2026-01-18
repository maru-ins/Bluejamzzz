#pragma once
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* OLED */
#define OLED_SDA 4
#define OLED_SCL 5
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void setupDisplay();
void bootScreen();
void drawCurrentState();

/* draw helpers */
void drawMenu(const char* header, const char* items[], int count);
void drawStatus(const char* title);
void drawMainMenuStatus();
