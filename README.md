# PDL_Addressable_LED

The `PDL_Addressable_LED` library provides an interface for controlling addressable LEDs using the Adafruit NeoPixel library. It includes various pre-defined patterns and allows for customization of single-color patterns with support for features like fading and marquee effects.

## Features
- Easy integration with Adafruit NeoPixel library
- Pre-defined LED patterns
- Custom single-color patterns
- Support for fading and marquee effects
- Debugging features

## Installation

1. Download the library from the GitHub repository.
2. Unzip the downloaded file.
3. Move the unzipped folder to your Arduino libraries directory (usually located in `Documents/Arduino/libraries`).

## File Structure

```
.
├── example
│   ├── Color_unit_test
│   │   ├── Color_unit_test.ino
│   │   └── readme.md
│   └── demo
│       └── demo.ino
├── keywords.txt
├── library.properties
├── LICENSE
├── README.md
└── src
    ├── Color.cpp
    ├── Color.hpp
    ├── PDL_Addressable_LED.cpp
    └── PDL_Addressable_LED.h

4 directories, 11 files
```

## Usage

### Include the Library

```cpp
#include <PDL_Addressable_LED.h>
```

### Initialize the LED Controller

```cpp
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
PDL_Addressable_LED ledController(strip);
```

### Set a Pre-defined Pattern

```cpp
ledController.setPatternSingleColor(PDL_Addressable_LED::PATTERN_GREEN_CONST_ALL);
```

### Create and Set a Custom Pattern

```cpp
PDL_Addressable_LED::single_color_pattern_t customPattern = {
    .r = 255,
    .g = 0,
    .b = 0,
    .w = 0,
    .index = 0,
    .count_mid = 5,
    .count_dim_left = 2,
    .count_dim_right = 2,
    .fade_interval_ms = 1000,
    .fade_step = 10,
    .marquee_interval_ms = 500,
    .marquee_dir = PDL_Addressable_LED::Marquee_t::CIRCULAR_LEFT
};

ledController.setPatternSingleColor(customPattern);
```

### Debugging

Enable debugging to get detailed logs:

```cpp
ledController.setDebug(1);
```

## API Reference

### Methods

- `PDL_Addressable_LED(Adafruit_NeoPixel &neoPixel)`
  Constructor to initialize the LED controller with the given NeoPixel object.

- `bool init()`
  Initialize the LED controller.

- `bool deinit()`
  Deinitialize the LED controller.

- `bool setPatternSingleColor(const single_color_pattern_t &pattern)`
  Set a single-color pattern.

- `void setDebug(uint8_t debug)`
  Enable or disable debugging.

## License

This library is licensed under the MIT License. See the LICENSE file for details.
