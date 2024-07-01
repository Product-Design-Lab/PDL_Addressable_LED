#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>
#include <cstddef>
#include <cmath>
#include <cstdio>
#include <type_traits>

class Color {
public:
    union {
        uint32_t u32;
        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t w;
        } channel;
    };

    // Default constructor
    Color();

    // Constructor to initialize color values
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);

    // Constructor to initialize using a uint32_t value
    Color(uint32_t value);

    // Overload the + operator (Color + Color)
    Color operator+(const Color& other) const;

    // Overload the - operator (Color - Color)
    Color operator-(const Color& other) const;

    // Templated function to handle various integer types and float types
    template<typename T>
    Color operator+(T value) const;

    template<typename T>
    Color operator-(T value) const;

    template<typename T>
    Color operator*(T value) const;

    template<typename T>
    Color operator/(T value) const;

    // Function to print color values to a buffer
    void toString(char* buffer, size_t bufferSize) const;

private:
    // Function to clamp values to uint8_t range
    static uint8_t clamp(int value);

    // Function to convert float to uint8_t with clamping
    static uint8_t floatToUint8(float value);

    // Templated function to handle various integer types and float types
    template<typename T>
    static uint8_t toUint8(T value);
};

// Inline implementations of template functions

template<typename T>
Color Color::operator+(T value) const {
    return Color(clamp(static_cast<int>(channel.r) + static_cast<int>(value)),
                 clamp(static_cast<int>(channel.g) + static_cast<int>(value)),
                 clamp(static_cast<int>(channel.b) + static_cast<int>(value)),
                 clamp(static_cast<int>(channel.w) + static_cast<int>(value)));
}

template<typename T>
Color Color::operator-(T value) const {
    return Color(clamp(static_cast<int>(channel.r) - static_cast<int>(value)),
                 clamp(static_cast<int>(channel.g) - static_cast<int>(value)),
                 clamp(static_cast<int>(channel.b) - static_cast<int>(value)),
                 clamp(static_cast<int>(channel.w) - static_cast<int>(value)));
}

template<typename T>
Color Color::operator*(T value) const {
    return Color(clamp(static_cast<int>(channel.r) * static_cast<int>(value)),
                 clamp(static_cast<int>(channel.g) * static_cast<int>(value)),
                 clamp(static_cast<int>(channel.b) * static_cast<int>(value)),
                 clamp(static_cast<int>(channel.w) * static_cast<int>(value)));
}

template<typename T>
Color Color::operator/(T value) const {
    return Color(clamp(static_cast<int>(channel.r) / static_cast<int>(value)),
                 clamp(static_cast<int>(channel.g) / static_cast<int>(value)),
                 clamp(static_cast<int>(channel.b) / static_cast<int>(value)),
                 clamp(static_cast<int>(channel.w) / static_cast<int>(value)));
}

template<typename T>
uint8_t Color::toUint8(T value) {
    if (std::is_floating_point<T>::value) {
        return floatToUint8(static_cast<float>(value));
    } else {
        return clamp(static_cast<int>(value));
    }
}

#endif // COLOR_HPP
