#ifndef PDL_ADDRESSABLE_LED_H
#define PDL_ADDRESSABLE_LED_H

#include "Adafruit_NeoPixel.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "Color.hpp"

class PDL_Addressable_LED
{
public:
    enum class Marquee_t : int8_t
    {
        BACKFORTH_LEFT = -2,
        CIRCULAR_LEFT = -1,
        OFF = 0,
        CIRCULAR_RIGHT = 1,
        BACKFORTH_RIGHT = 2
    };

    constexpr static uint8_t COLOR_W_UNUSE = 0;
    constexpr static uint8_t INDEX_UNCHANGE = 255; // Fixed typo
    constexpr static uint8_t MARQUE_INTERVAL_DISABLE = 0;
    constexpr static uint8_t FADE_DISABLE = 0;
    constexpr static uint8_t MARQUEE_DISABLE = 0;

    struct single_color_pattern_t
    {
        uint8_t r;                    // red channel of the center led
        uint8_t g;                    // green channel of the center led
        uint8_t b;                    // blue channel of the center led
        uint8_t w;                    // white channel of the center led, set to 0 to disable
        uint8_t index;                // index of the initial center led, set to 255 to use existing index
        uint8_t count_mid;            // number of leds to light up, 255 for all
        uint8_t count_dim_left;       // number of leds to the left of the center led
        uint8_t count_dim_right;      // number of leds to the right of the center led
        uint16_t fade_interval_ms;    // fade of the center led 0 to disable
        uint8_t fade_step;            // number of steps to fade, 0 to disable, 1 to blink on/off
        uint16_t marquee_interval_ms; // zero to disable
        int8_t marquee_dir;           // direction of the marquee
    };

    // Predefined patterns
    static const single_color_pattern_t PATTERN_OFF;
    static const single_color_pattern_t PATTERN_GREEN_CONST_ALL;
    static const single_color_pattern_t PATTERN_ORANGE_DIM_DEFAULT_INDEX;
    static const single_color_pattern_t PATTERN_RED_FADE_ALL;
    static const single_color_pattern_t PATTERN_GREEN_MARQUEE_CIRCULAR;

    PDL_Addressable_LED(Adafruit_NeoPixel &neoPixel);
    virtual ~PDL_Addressable_LED();

    bool init();
    bool deinit();

    bool setPatternSingleColor(const single_color_pattern_t &pattern);

    void setDebug(uint8_t debug);

private:
    static constexpr int MAX_LED_COUNT = 32;
    const int led_count;

    uint8_t debug = 0;

    Color color[MAX_LED_COUNT];

    uint8_t center_led_index = 0;
    Adafruit_NeoPixel &neoPixel;
    single_color_pattern_t current_pattern = PATTERN_OFF;
    QueueHandle_t xQueue = NULL;
    TaskHandle_t xTask = NULL;

    // Task functions
    static void _task_wrapper(void *pvParameters);
    void _patternLoop();
    // pattern will be applied from left to right
    void _PatternApplyTransition(const Color cLeft, const Color cRight, int uLeft, int uRight, bool extendLeft = false, bool extendRight = false);
    void _PatternFillColor(const Color c, int uStart, int uEnd);
    void _patternInit(const single_color_pattern_t &pattern);
    void _patternShow(int left_shift_count = 0, float fade_factor = 1.0f);

    void printPattern(const single_color_pattern_t &pattern);
    void debugPrintf(const char *format, ...);
};

#endif // PDL_ADDRESSABLE_LED_H
