#include "custom/clockwise_component.h"
// Show the startup logo (uses Clockwise StatusController if present)
StatusController::getInstance()->clockwiseLogo();
delay(1000);
StatusController::getInstance()->wifiConnecting();


// Start WiFi controller (Clockwise has its own WiFi helpers); ESPHome already brought up WiFi,
// but the Clockwise WiFiController may be used for Improv / webserver handling inside the
// Clockwise codebase. We'll still call begin() to let it initialize its state.
if (wifi_.begin()) {
// Use the CWDateTime helper with timezone/format settings from ClockwiseParams
cwDateTime_.begin(
ClockwiseParams::getInstance()->timeZone.c_str(),
ClockwiseParams::getInstance()->use24hFormat,
ClockwiseParams::getInstance()->ntpServer.c_str(),
ClockwiseParams::getInstance()->manualPosix.c_str()
);


// Create and initialize the clockface drawing object
clockface_ = new Clockface(dma_display_);
clockface_->setup(&cwDateTime_);
}
}


void ClockwiseComponent::loop() {
// Let the Clockwise WiFi helper process events if present
wifi_.handleImprovWiFi();


if (wifi_.isConnected()) {
ClockwiseWebServer::getInstance()->handleHttpRequest();
ezt::events();
}


if (wifi_.connectionSucessfulOnce) {
// Advance the clockface update (animations / redraws)
if (clockface_)
clockface_->update();
}


// Manage automatic brightness (if enabled)
automaticBrightControl();
}


void ClockwiseComponent::automaticBrightControl() {
if (!autoBrightEnabled_)
return;


if (millis() - autoBrightMillis_ > 3000) {
int16_t currentValue = analogRead(ldr_pin_);
uint16_t ldrMin = ClockwiseParams::getInstance()->autoBrightMin;
uint16_t ldrMax = ClockwiseParams::getInstance()->autoBrightMax;
const uint8_t MIN_BRIGHT_DISPLAY_OFF = 0; // fallback if not defined in scope
const uint8_t MIN_BRIGHT_DISPLAY_ON = 8; // fallback if not defined in scope


const uint8_t minBright = (currentValue < ldrMin ? MIN_BRIGHT_DISPLAY_OFF : MIN_BRIGHT_DISPLAY_ON);
uint8_t maxBright = ClockwiseParams::getInstance()->displayBright;
uint8_t slots = 10;
int val = currentValue > ldrMax ? ldrMax : currentValue;
uint8_t mapLDR = map(val, ldrMin, ldrMax, 1, slots);
uint8_t mapBright = map(mapLDR, 1, slots, minBright, maxBright);


if (abs((int)currentBrightSlot_ - (int)mapLDR) >= 2 || mapBright == 0) {
if (dma_display_)
dma_display_->setBrightness8(mapBright);
currentBrightSlot_ = mapLDR;
}


autoBrightMillis_ = millis();
}
}