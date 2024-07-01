#include <Arduino.h>
#include "Color.hpp"
#include "Adafruit_TinyUSB.h"

void runTests();
void testColorAddition();
void testColorSubtraction();
void testColorMultiplication();
void testColorDivision();
void testColorAdditionWithInt();
void testColorSubtractionWithInt();
void testColorAdditionWithFloat();
void testColorSubtractionWithFloat();
void assertEqual(uint8_t actual, uint8_t expected, const char* testName);
void assertEqual(uint32_t actual, uint32_t expected, const char* testName);

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ; // Wait for serial port to connect. Needed for native USB
    }
    Serial.println("Starting unit tests...");
    runTests();
}

void loop() {
    // Empty loop
}

void runTests() {
    testColorAddition();
    testColorSubtraction();
    testColorMultiplication();
    testColorDivision();
    testColorAdditionWithInt();
    testColorSubtractionWithInt();
    testColorAdditionWithFloat();
    testColorSubtractionWithFloat();
}

void testColorAddition() {
    Color color1(100, 150, 200, 50);
    Color color2(50, 100, 55, 100);
    Color result = color1 + color2;

    assertEqual(result.channel.r, 150, "colorAddition r");
    assertEqual(result.channel.g, 250, "colorAddition g");
    assertEqual(result.channel.b, 255, "colorAddition b"); // Clamped to 255
    assertEqual(result.channel.w, 150, "colorAddition w");
}

void testColorSubtraction() {
    Color color1(100, 150, 200, 50);
    Color color2(50, 100, 55, 100);
    Color result = color1 - color2;

    assertEqual(result.channel.r, 50, "colorSubtraction r");
    assertEqual(result.channel.g, 50, "colorSubtraction g");
    assertEqual(result.channel.b, 145, "colorSubtraction b");
    assertEqual(result.channel.w, 0, "colorSubtraction w"); // Clamped to 0
}

void testColorMultiplication() {
    Color color1(100, 150, 200, 50);
    int multiplier = 2;
    Color result = color1 * multiplier;

    assertEqual(result.channel.r, 200, "colorMultiplication r");
    assertEqual(result.channel.g, 255, "colorMultiplication g"); // Clamped to 255
    assertEqual(result.channel.b, 255, "colorMultiplication b"); // Clamped to 255
    assertEqual(result.channel.w, 100, "colorMultiplication w");
}

void testColorDivision() {
    Color color1(100, 150, 200, 50);
    int divisor = 2;
    Color result = color1 / divisor;

    assertEqual(result.channel.r, 50, "colorDivision r");
    assertEqual(result.channel.g, 75, "colorDivision g");
    assertEqual(result.channel.b, 100, "colorDivision b");
    assertEqual(result.channel.w, 25, "colorDivision w");
}

void testColorAdditionWithInt() {
    Color color1(100, 150, 200, 50);
    int addValue = 55;
    Color result = color1 + addValue;

    assertEqual(result.channel.r, 155, "colorAdditionWithInt r");
    assertEqual(result.channel.g, 205, "colorAdditionWithInt g");
    assertEqual(result.channel.b, 255, "colorAdditionWithInt b"); // Clamped to 255
    assertEqual(result.channel.w, 105, "colorAdditionWithInt w");
}

void testColorSubtractionWithInt() {
    Color color1(100, 150, 200, 50);
    int subValue = 55;
    Color result = color1 - subValue;

    assertEqual(result.channel.r, 45, "colorSubtractionWithInt r");
    assertEqual(result.channel.g, 95, "colorSubtractionWithInt g");
    assertEqual(result.channel.b, 145, "colorSubtractionWithInt b");
    assertEqual(result.channel.w, 0, "colorSubtractionWithInt w"); // Clamped to 0
}

void testColorAdditionWithFloat() {
    Color color1(100, 150, 200, 50);
    float addValue = 55.5;
    Color result = color1 + addValue;

    assertEqual(result.channel.r, 155, "colorAdditionWithFloat r");
    assertEqual(result.channel.g, 205, "colorAdditionWithFloat g");
    assertEqual(result.channel.b, 255, "colorAdditionWithFloat b"); // Clamped to 255
    assertEqual(result.channel.w, 105, "colorAdditionWithFloat w");
}

void testColorSubtractionWithFloat() {
    Color color1(100, 150, 200, 50);
    float subValue = 55.5;
    Color result = color1 - subValue;

    assertEqual(result.channel.r, 45, "colorSubtractionWithFloat r");
    assertEqual(result.channel.g, 95, "colorSubtractionWithFloat g");
    assertEqual(result.channel.b, 145, "colorSubtractionWithFloat b");
    assertEqual(result.channel.w, 0, "colorSubtractionWithFloat w"); // Clamped to 0
}

void assertEqual(uint8_t actual, uint8_t expected, const char* testName) {
    if (actual == expected) {
        Serial.print(testName);
        Serial.println(": PASS");
    } else {
        Serial.print(testName);
        Serial.print(": FAIL (expected ");
        Serial.print(expected);
        Serial.print(", got ");
        Serial.print(actual);
        Serial.println(")");
    }
}

void assertEqual(uint32_t actual, uint32_t expected, const char* testName) {
    if (actual == expected) {
        Serial.print(testName);
        Serial.println(": PASS");
    } else {
        Serial.print(testName);
        Serial.print(": FAIL (expected ");
        Serial.print(expected);
        Serial.print(", got ");
        Serial.print(actual);
        Serial.println(")");
    }
}
