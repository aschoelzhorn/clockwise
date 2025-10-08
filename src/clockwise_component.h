#pragma once


#include "esphome.h"
#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"


// The Clockwise project defines classes such as Clockface, ClockwiseParams, CWDateTime, etc.
// Make sure the Clockwise sources are available to the build (see YAML libraries or copy sources).
#include "Clockface.h"
#include "ClockwiseParams.h"
#include "CWDateTime.h"
#include "WiFiController.h"
#include "ClockwiseWebServer.h"
#include "StatusController.h"


class ClockwiseComponent : public Component {
public:
ClockwiseComponent(int width, int height, int chain, bool swap_blue_green,
uint8_t display_brightness, uint8_t rotation, int ldr_pin,
time::RealTimeClock *rtc)
: width_(width), height_(height), chain_(chain), swap_blue_green_(swap_blue_green),
display_brightness_(display_brightness), rotation_(rotation), ldr_pin_(ldr_pin), rtc_(rtc) {}


void setup() override;
void loop() override;


protected:
int width_, height_, chain_;
bool swap_blue_green_;
uint8_t display_brightness_, rotation_;
int ldr_pin_;
time::RealTimeClock *rtc_;


MatrixPanel_I2S_DMA *dma_display_{nullptr};
Clockface *clockface_{nullptr};
CWDateTime cwDateTime_;
WiFiController wifi_;
bool autoBrightEnabled_{false};
unsigned long autoBrightMillis_{0};
uint8_t currentBrightSlot_{0};


void automaticBrightControl();
};