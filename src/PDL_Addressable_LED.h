#pragma once

#include "Adafruit_NeoPixel.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

class PDL_Addressable_LED
{

public:
    typedef enum
    {
        MARQUEE_BACKFORTH_LEFT = -2,
        MARQUEE_CIRCULAR_LEFT = -1,
        MARQUEE_OFF = 0,
        MARQUEE_CIRCULAR_RIGHT = 1,
        MARQUEE_BACKFORTH_RIGHT = 2
    } Marquee_t;
    typedef struct
    {
        uint32_t color;              // color of the center led
        uint8_t index;               // index of the initial center led
        uint8_t count;               // number of leds to light up, 255 for all
        uint8_t dim_count_left;     // number of leds to the left of the center led
        uint8_t dim_count_right;    // number of leds to the right of the center led
        uint8_t fade_interval_ms;    // fade of the center led zero to disable, 1 to blink on/off
        uint8_t fade_step;           // number of steps to fade
        uint8_t marquee_interval_ms; // zero to disable
        int8_t marquee_dir;          // direction of the marquee
    } single_color_pattern_t;

    // predefine patterns
    static const single_color_pattern_t PATTERN_OFF = {
        .color = 0x000000,
        .index = 0,
        .count = 0,
        .dim_count_left = 0,
        .dim_count_right = 0,
        .fade_interval_ms = 0,
        .fade_step = 0,
        .marquee_interval_ms = 0,
        .marquee_dir = MARQUEE_OFF};

    static const single_color_pattern_t PATTERN_GREEN_CONST_ALL = {
        .color = 0x00FF00,
        .index = 0,
        .count = 255,
        .dim_count_left = 0,
        .dim_count_right = 0,
        .fade_interval_ms = 0,
        .fade_step = 0,
        .marquee_interval_ms = 0,
        .marquee_dir = MARQUEE_OFF};

    static const single_color_pattern_t PATTERN_ORANGE_DIM_DEFAULT_INDEX = {// need to change index
        .color = 0xFFA500,
        .index = 0,
        .count = 0,
        .dim_count_left = 0,
        .dim_count_right = 0,
        .fade_interval_ms = 0,
        .fade_step = 0,
        .marquee_interval_ms = 0,
        .marquee_dir = MARQUEE_OFF};

    static const single_color_pattern_t PATTERN_RED_FADE_ALL = {
        .color = 0xFF0000,
        .index = 0,
        .count = 255,
        .dim_count_left = 0,
        .dim_count_right = 0,
        .fade_interval_ms = 25,
        .fade_step = 10,
        .marquee_interval_ms = 0,
        .marquee_dir = MARQUEE_OFF};
    
    static const single_color_pattern_t PATTERN_GREEN_MARQUEE_CIRCULAR = {
        .color = 0x00FF00,
        .index = 0,
        .count = 1,
        .dim_count_left = 1,
        .dim_count_right = 1,
        .fade_interval_ms = 0,
        .fade_step = 0,
        .marquee_interval_ms = 100,
        .marquee_dir = MARQUEE_CIRCULAR_RIGHT};

    PDL_Addressable_LED(Adafruit_NeoPixel &neoPixel);
    ~PDL_Addressable_LED();

    bool init();
    bool deinit();

    bool setPatternSingleColor(single_color_pattern_t &pattern);

private:
    constexpr static uint32_t MAX_LED_COUNT = 32;
    // neoPixel object by reference.
    Adafruit_NeoPixel &neoPixel;
    single_color_pattern_t current_pattern;

    QueueHandle_t xQueue;
    TaskHandle_t xTask;
    Adafruit_NeoPixel::Color color_map[MAX_LED_COUNT];
    uint32_t led_count;
    uint32_t center_led_index;
}