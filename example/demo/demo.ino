#include <Arduino.h>
#include <PDL_Addressable_LED.h>

#define NUM_LED 12
#define PIN_NEOPIXEL_LED 10

Adafruit_NeoPixel strip(NUM_LED, PIN_NEOPIXEL_LED, NEO_GRB + NEO_KHZ800);

PDL_Addressable_LED led_strip(strip);

PDL_Addressable_LED::single_color_pattern_t patternList[] = {
    PDL_Addressable_LED::PATTERN_OFF,
    PDL_Addressable_LED::PATTERN_RED_CONST_ALL,
    PDL_Addressable_LED::PATTERN_GREEN_CONST_ALL,
    PDL_Addressable_LED::PATTERN_BLUE_CONST_ALL,
    PDL_Addressable_LED::PATTERN_RED_FADE_ALL,
    PDL_Addressable_LED::PATTERN_GREEN_FADE_ALL,
    PDL_Addressable_LED::PATTERN_BLUE_FADE_ALL,
    PDL_Addressable_LED::PATTERN_RED_MARQUEE_CIRCULAR,
    PDL_Addressable_LED::PATTERN_GREEN_MARQUEE_CIRCULAR,
    PDL_Addressable_LED::PATTERN_BLUE_MARQUEE_CIRCULAR,
    PDL_Addressable_LED::PATTERN_ORANGE_DIM_DEFAULT_INDEX};

PDL_Addressable_LED::single_color_pattern_t pat = {
    .r = 255,
    .g = 0,
    .b = 0,
    .w = 0,
    .index = 0,
    .count_mid = 1,
    .count_dim_left = 1,
    .count_dim_right = 1,
    .fade_interval_ms = 25,
    .fade_step = 20,
    .marquee_interval_ms = 0,
    .marquee_dir = 0};

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000)
    ;
  led_strip.setDebug(1);
  led_strip.init();
}

void loop()
{
  // iterate through the pattern list
  for (int i = 0; i < sizeof(patternList) / sizeof(patternList[0]); i++)
  {
    led_strip.setPatternSingleColor(patternList[i]);
    delay(1000);
  }

  led_strip.setPatternSingleColor(pat);
  for (int i = 0; i < 10 * NUM_LED; i++)
  {
    pat.index = i % NUM_LED;
    led_strip.setPatternSingleColor(pat);
    delay(100);
  }
}
