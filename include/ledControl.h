#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define LEDS_PIN 14 //D5 
#define N_LEDS 12

void init_leds();
void blankLeds();
void countdown();
void enableLed(int led);
void disableLed(int led);