#include "ledControl.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

void init_leds() {
    strip.begin();
}

void enableLed(int led) {

    if (led < N_LEDS) {
        uint32_t myColor = strip.Color(0,0,255);
        strip.setPixelColor(led, myColor);
        strip.show();
    }
}

void disableLed(int led) {

  if (led > 0 && led < N_LEDS) {
    strip.setPixelColor(led, 0);
    strip.show();
  }
}

void blankLeds() {
  for (int i=0; i < N_LEDS; i++) {
    strip.setPixelColor(i,0);
  }
  strip.show();
}

void countdown() {

  uint32_t myColor = strip.Color(255,0,0);
  for (int i=0; i<6; i++) {
    strip.setPixelColor(i, myColor);
    strip.setPixelColor(11-i, myColor);
    strip.show();
    delay(750);
  }

  myColor = strip.Color(0, 255, 0);
  for (int i=0; i<6; i++) {
    strip.setPixelColor(i, myColor);
    strip.setPixelColor(11-i, myColor);
  }
  strip.show();
}

static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
  }
}