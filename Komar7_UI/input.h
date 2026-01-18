#pragma once
#include <Arduino.h>

/* BUTTON */
#define BTN_SELECT 25
#define BTN_UP     26
#define BTN_DOWN   27
#define BTN_BACK   32

void setupInput();
bool buttonPressed(uint8_t pin);
void handleNavigation();
