#include "Color.hpp"

// Default constructor
Color::Color() : u32(0) {}

// Constructor to initialize color values
Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    channel.r = red;
    channel.g = green;
    channel.b = blue;
    channel.w = white;
}

// Constructor to initialize using a uint32_t value
Color::Color(uint32_t value) : u32(value) {}

// Overload the + operator (Color + Color)
Color Color::operator+(const Color& other) const {
    return Color(clamp(static_cast<int>(channel.r) + static_cast<int>(other.channel.r)),
                 clamp(static_cast<int>(channel.g) + static_cast<int>(other.channel.g)),
                 clamp(static_cast<int>(channel.b) + static_cast<int>(other.channel.b)),
                 clamp(static_cast<int>(channel.w) + static_cast<int>(other.channel.w)));
}

// Overload the - operator (Color - Color)
Color Color::operator-(const Color& other) const {
    return Color(clamp(static_cast<int>(channel.r) - static_cast<int>(other.channel.r)),
                 clamp(static_cast<int>(channel.g) - static_cast<int>(other.channel.g)),
                 clamp(static_cast<int>(channel.b) - static_cast<int>(other.channel.b)),
                 clamp(static_cast<int>(channel.w) - static_cast<int>(other.channel.w)));
}

// Function to print color values to a buffer
void Color::toString(char* buffer, size_t bufferSize) const {
    snprintf(buffer, bufferSize, "Color: (0x%08X, %d, %d, %d, %d)", u32, channel.r, channel.g, channel.b, channel.w);
}

// Function to clamp values to uint8_t range
uint8_t Color::clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return static_cast<uint8_t>(value);
}

// Function to convert float to uint8_t with clamping
uint8_t Color::floatToUint8(float value) {
    int intValue = static_cast<int>(std::round(value));
    return clamp(intValue);
}
