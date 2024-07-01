#include "PDL_Addressable_LED.h"

// Constants for task configuration
#define TASK_PRIORITY 1
#define TASK_STACK_SIZE 1024
#define QUEUE_LENGTH 10

// LED Patterns
const PDL_Addressable_LED::single_color_pattern_t PDL_Addressable_LED::PATTERN_OFF = {
    .r = 0, .g = 0, .b = 0, .w = 0, .index = 0, .count_mid = 0, .count_dim_left = 0, .count_dim_right = 0, .fade_interval_ms = 0, .fade_step = 0, .marquee_interval_ms = 0, .marquee_dir = static_cast<int8_t>(Marquee_t::OFF)};

const PDL_Addressable_LED::single_color_pattern_t PDL_Addressable_LED::PATTERN_GREEN_CONST_ALL = {
    .r = 0, .g = 255, .b = 0, .w = 0, .index = 0, .count_mid = 255, .count_dim_left = 0, .count_dim_right = 0, .fade_interval_ms = 0, .fade_step = 0, .marquee_interval_ms = 0, .marquee_dir = static_cast<int8_t>(Marquee_t::OFF)};

const PDL_Addressable_LED::single_color_pattern_t PDL_Addressable_LED::PATTERN_ORANGE_DIM_DEFAULT_INDEX = {
    .r = 255,
    .g = 0,
    .b = 0,
    .w = 0,
    .index = 3,
    .count_mid = 1,
    .count_dim_left = 2,
    .count_dim_right = 2,
    .fade_interval_ms = 50,
    .fade_step = 10,
    .marquee_interval_ms = 0,
    .marquee_dir = static_cast<int8_t>(Marquee_t::OFF)};

const PDL_Addressable_LED::single_color_pattern_t PDL_Addressable_LED::PATTERN_RED_FADE_ALL = {
    .r = 255,
    .g = 0,
    .b = 0,
    .w = 0,
    .index = 0,
    .count_mid = 255,
    .count_dim_left = 0,
    .count_dim_right = 0,
    .fade_interval_ms = 25,
    .fade_step = 10,
    .marquee_interval_ms = 0,
    .marquee_dir = static_cast<int8_t>(Marquee_t::OFF)};

const PDL_Addressable_LED::single_color_pattern_t PDL_Addressable_LED::PATTERN_GREEN_MARQUEE_CIRCULAR = {
    .r = 0, .g = 255, .b = 0, .w = 0, .index = 0, .count_mid = 1, .count_dim_left = 1, .count_dim_right = 1, .fade_interval_ms = 0, .fade_step = 0, .marquee_interval_ms = 100, .marquee_dir = static_cast<int8_t>(Marquee_t::CIRCULAR_RIGHT)};

// Constructor
PDL_Addressable_LED::PDL_Addressable_LED(Adafruit_NeoPixel &neoPixel)
    : neoPixel(neoPixel),
      current_pattern(PATTERN_OFF),
      xQueue(nullptr),
      xTask(nullptr),
      led_count(neoPixel.numPixels()),
      center_led_index(0)
{
}

// Destructor
PDL_Addressable_LED::~PDL_Addressable_LED()
{
    deinit();
}

// Initialization
bool PDL_Addressable_LED::init()
{
    neoPixel.begin();
    neoPixel.fill(0, 0, led_count);
    neoPixel.show();

    // Initialize queue
    if (!xQueue)
    {
        xQueue = xQueueCreate(QUEUE_LENGTH, sizeof(single_color_pattern_t));
        if (!xQueue)
        {
            return false;
        }
    }

    // Create task
    if (!xTask)
    {
        if (xTaskCreate(_task_wrapper, "NeoPixel Task", TASK_STACK_SIZE, this, TASK_PRIORITY, &xTask) != pdPASS)
        {
            return false;
        }
    }

    return true;
}

// De-initialization
bool PDL_Addressable_LED::deinit()
{
    if (xTask)
    {
        vTaskDelete(xTask);
        xTask = nullptr;
    }

    if (xQueue)
    {
        vQueueDelete(xQueue);
        xQueue = nullptr;
    }

    neoPixel.clear();
    neoPixel.show();

    return true;
}

// Set single color pattern
bool PDL_Addressable_LED::setPatternSingleColor(const single_color_pattern_t &pattern)
{
    if (xQueueSend(xQueue, &pattern, 0) != pdPASS)
    {
        return false; // Failed to post the message
    }
    return true;
}

// Task wrapper
void PDL_Addressable_LED::_task_wrapper(void *pvParameters)
{
    PDL_Addressable_LED *pInstance = static_cast<PDL_Addressable_LED *>(pvParameters);
    pInstance->_patternLoop();
}

// Pattern loop
void PDL_Addressable_LED::_patternLoop()
{
    single_color_pattern_t receivedPattern;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    TickType_t waitTime = pdMS_TO_TICKS(50);
    TickType_t marqueeLastRunTime = 0;
    TickType_t fadeLastRunTime = 0;
    int fadeCurrentStep = 0;
    float fade_factor = 1.0f;
    int marqueeCurrentStep = 0;
    int8_t left_shift_count = 0;

    while (true)
    {
        if (xQueueReceive(xQueue, &receivedPattern, 0) == pdPASS)
        {
            _patternInit(receivedPattern);
            debugPrintf("New pattern received\n");
            // printPattern(receivedPattern);
        }

        // Handle fading
        if (current_pattern.fade_interval_ms && current_pattern.fade_step &&
            xLastWakeTime - fadeLastRunTime >= pdMS_TO_TICKS(current_pattern.fade_interval_ms))
        {
            fadeLastRunTime = xTaskGetTickCount();
            fadeCurrentStep = (fadeCurrentStep + 1) % (current_pattern.fade_step * 2);
            fade_factor = abs(fadeCurrentStep - current_pattern.fade_step) / static_cast<float>(current_pattern.fade_step);
            // debugPrintf("Fade triggered: factor: %f\n", fade_factor);
        }

        // Handle marquee
        if (current_pattern.marquee_interval_ms && current_pattern.marquee_dir != static_cast<int8_t>(Marquee_t::OFF) &&
            xLastWakeTime - marqueeLastRunTime >= pdMS_TO_TICKS(current_pattern.marquee_interval_ms))
        {
            marqueeLastRunTime = xTaskGetTickCount();
            switch (static_cast<Marquee_t>(current_pattern.marquee_dir))
            {
            case Marquee_t::CIRCULAR_RIGHT:
                left_shift_count = (left_shift_count + 1) % led_count;
                break;
            case Marquee_t::CIRCULAR_LEFT:
                left_shift_count = (left_shift_count - 1) % led_count;
                break;
            case Marquee_t::BACKFORTH_RIGHT:
                marqueeCurrentStep = (marqueeCurrentStep + 1) % (2 * led_count);
                left_shift_count = marqueeCurrentStep - led_count;
                break;
            case Marquee_t::BACKFORTH_LEFT:
                marqueeCurrentStep = (marqueeCurrentStep - 1) % (2 * led_count);
                left_shift_count = marqueeCurrentStep - led_count;
                break;
            default:
                break;
            }
            debugPrintf("Marquee triggered: left_shift_count: %d\n", left_shift_count);
        }

        _patternShow(left_shift_count, fade_factor);

        // Calculate wait time
        TickType_t fadeTimeLeft = current_pattern.fade_interval_ms == 0 ? 50 : pdMS_TO_TICKS(current_pattern.fade_interval_ms) - (xLastWakeTime - fadeLastRunTime);
        TickType_t marqueeTimeLeft = current_pattern.marquee_interval_ms == 0 ? 50 : pdMS_TO_TICKS(current_pattern.marquee_interval_ms) - (xLastWakeTime - marqueeLastRunTime);
        waitTime = min(fadeTimeLeft, marqueeTimeLeft);
        // debugPrintf("waitTime: %d, fadeTimeLeft: %d, marqueeTimeLeft: %d\n", waitTime, fadeTimeLeft, marqueeTimeLeft);

        vTaskDelayUntil(&xLastWakeTime, waitTime);
    }

    // Should never reach here
    vTaskDelete(nullptr);
}

void PDL_Addressable_LED::_PatternFillColor(const Color c, int uLeft, int uRight)
{
    uLeft = (uLeft + led_count) % led_count;
    uRight = (uRight + led_count) % led_count;
    int range = (uRight - uLeft + led_count) % led_count;

    debugPrintf("_PatternFillColor, uLeft: %d, uRight: %d\n", uLeft, uRight);

    for (int i = 0; i <= range; i++)
    {
        color[(uLeft + i) % led_count] = c;
    }
}

void PDL_Addressable_LED::_PatternApplyTransition(const Color cLeft, const Color cRight, int uLeft, int uRight, bool extendLeft, bool extendRight)
{
    uLeft = (uLeft + led_count) % led_count;
    uRight = (uRight + led_count) % led_count;

    int range = (uRight - uLeft + led_count) % led_count;

    if (range == 0)
    {
        return;
    }

    int extendedRange = range + (int)extendLeft + (int)extendRight;
    int rStep = (cRight.channel.r - cLeft.channel.r) / extendedRange;
    int gStep = (cRight.channel.g - cLeft.channel.g) / extendedRange;
    int bStep = (cRight.channel.b - cLeft.channel.b) / extendedRange;
    int wStep = (cRight.channel.w - cLeft.channel.w) / extendedRange;
    // print rStep and range
    debugPrintf("_PatternApplyTransition, rStep: %d, uLeft: %d, uRight: %d, range: %d, extendedRange: %d\n", rStep, uLeft, uRight, range, extendedRange);

    // determine loop dir based on signness of range
    for (int i = 0; i <= range; i++)
    {
        int index = (uLeft + i) % led_count;
        Color linearMapped = Color(cLeft.channel.r + rStep * (i + (int)extendLeft),
                                   cLeft.channel.g + gStep * (i + (int)extendLeft),
                                   cLeft.channel.b + bStep * (i + (int)extendLeft),
                                   cLeft.channel.w + wStep * (i + (int)extendLeft));

        color[index] = linearMapped; // gamma correct will be done in _patternShow
    }
}

// Pattern initialization
void PDL_Addressable_LED::_patternInit(const single_color_pattern_t &pattern)
{
    memset(color, 0, sizeof(color));
    current_pattern = pattern;

    if (pattern.index != 255)
    {
        center_led_index = pattern.index;
    }

    Color mainColor(pattern.r, pattern.g, pattern.b, pattern.w);
    Color off(0, 0, 0, 0);
    // find leftMostIndex
    int leftMostIndex = center_led_index - pattern.count_dim_left; // allow the most left LED to also be lit
    int midStartIndex = center_led_index;
    int midEndIndex = center_led_index + pattern.count_mid - 1;
    int rightMostIndex = midEndIndex + pattern.count_dim_right;

    // if marquee back and forth, dont wrap color to the other side
    if (pattern.marquee_dir == static_cast<int8_t>(Marquee_t::BACKFORTH_LEFT) ||
        pattern.marquee_dir == static_cast<int8_t>(Marquee_t::BACKFORTH_RIGHT))
    {
        leftMostIndex = max(leftMostIndex, 0);
        rightMostIndex = min(rightMostIndex, led_count - 1);
    }

    debugPrintf("leftMostIndex: %d, midStartIndex: %d, midEndIndex: %d, rightMostIndex: %d\n", leftMostIndex, midStartIndex, midEndIndex, rightMostIndex);

    if (pattern.count_dim_left != 0)
    {
        _PatternApplyTransition(off, mainColor, leftMostIndex, midStartIndex - 1, true, true);
    }

    if (pattern.count_mid > led_count)
    {
        _PatternFillColor(mainColor, 0, led_count - 1);
    }
    else
    {
        _PatternFillColor(mainColor, midStartIndex, midEndIndex);
    }

    if (pattern.count_dim_right != 0)
    {
        _PatternApplyTransition(mainColor, off, midEndIndex + 1, rightMostIndex, true, true);
    }

    debugPrintf("Color: %08X|%08X|%08X|%08X|%08X|%08X|%08X|%08X\n", color[0].u32, color[1].u32, color[2].u32, color[3].u32, color[4].u32, color[5].u32, color[6].u32, color[7].u32);

    _patternShow(0, 1.0f);
}

// Display pattern with shift and fade effects
void PDL_Addressable_LED::_patternShow(int left_shift_count, float fade_factor)
{
    // static char buffer[128];
    for (int i = 0; i < led_count; i++)
    {
        uint8_t running_index = (i - left_shift_count + led_count) % led_count;
        uint8_t r = neoPixel.gamma8((color[running_index].channel.r * fade_factor));
        uint8_t g = neoPixel.gamma8((color[running_index].channel.g * fade_factor));
        uint8_t b = neoPixel.gamma8((color[running_index].channel.b * fade_factor));
        uint8_t w = neoPixel.gamma8((color[running_index].channel.w * fade_factor));
        // print dimmed color
        // debugPrintf("Dimmed Color: i:%d, fade_factor:%0.2f, r:%d, g:%d, b:%d\n", i, fade_factor, r, g, b);
        neoPixel.setPixelColor(i, r, g, b, w);
    }

    // debugPrintf("Shift: %d, %04X|%04X|%04X|%04X|%04X|%04X|%04X|%04X\n", left_shift_count, neoPixel.getPixelColor(0), neoPixel.getPixelColor(1), neoPixel.getPixelColor(2), neoPixel.getPixelColor(3), neoPixel.getPixelColor(4), neoPixel.getPixelColor(5), neoPixel.getPixelColor(6), neoPixel.getPixelColor(7));
    // debugPrintf("\n");

    neoPixel.show();
}

// Set debug mode
void PDL_Addressable_LED::setDebug(uint8_t debug)
{
    this->debug = debug;
}

// Print pattern details
void PDL_Addressable_LED::printPattern(const single_color_pattern_t &pattern)
{
    if (debug == 1)
    {
        Serial.printf("Pattern: r:%d, g:%d, b:%d, w:%d, index:%d, count_mid:%d, count_dim_left:%d, count_dim_right:%d, fade_interval_ms:%d, fade_step:%d, marquee_interval_ms:%d, marquee_dir:%d\n",
                      pattern.r, pattern.g, pattern.b, pattern.w, pattern.index, pattern.count_mid, pattern.count_dim_left, pattern.count_dim_right, pattern.fade_interval_ms, pattern.fade_step, pattern.marquee_interval_ms, pattern.marquee_dir);
    }
}

// Print debug information
void PDL_Addressable_LED::debugPrintf(const char *format, ...)
{
    if (debug == 1)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}
