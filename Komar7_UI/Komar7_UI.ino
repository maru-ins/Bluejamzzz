#include "state.h"
#include "input.h"
#include "display_ui.h"

void setup() {
  setupInput();
  setupDisplay();

  bootScreen();
  mainMenuStart = millis();
}

void loop() {
  handleNavigation();
  drawCurrentState();
  delay(50);
}
