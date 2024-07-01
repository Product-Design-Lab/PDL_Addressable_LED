#include <Arduino.h>
#include <PDL_Addressable_LED.h>

#define NUM_LED 8
#define PIN_NEOPIXEL_LED 10

Adafruit_NeoPixel strip(NUM_LED, PIN_NEOPIXEL_LED, NEO_GRB + NEO_KHZ800);

PDL_Addressable_LED led_strip(strip);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  led_strip.setDebug(1);
  led_strip.init();
  // led_strip.setPatternSingleColor(PDL_Addressable_LED::PATTERN_ORANGE_DIM_DEFAULT_INDEX);
  
}
void loop() {
  // led_strip.setPatternSingleColor(PDL_Addressable_LED::PATTERN_OFF);
  // delay(5000);
  // led_strip.setPatternSingleColor(PDL_Addressable_LED::PATTERN_GREEN_CONST_ALL);
  // delay(5000);
  led_strip.setPatternSingleColor(PDL_Addressable_LED::PATTERN_ORANGE_DIM_DEFAULT_INDEX);
  delay(5000);
  led_strip.setPatternSingleColor(PDL_Addressable_LED::PATTERN_RED_FADE_ALL);
  delay(5000);
  // led_strip.setPatternSingleColor(PDL_Addressable_LED::PATTERN_GREEN_MARQUEE_CIRCULAR);
  // delay(5000);
}
