#include "tft_display_lib.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

// #if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
//   #define TFT_CS         14
//   #define TFT_RST        15
//   #define TFT_DC         32
// #else
//   // For the breakout board, you can use any 2 or 3 pins.
//   // These pins will also work for the 1.8" TFT shield.
//   #define TFT_CS        16
//   #define TFT_RST        -1 // Or set to -1 and connect to Arduino RESET pin
//   #define TFT_DC         15
// #endif



// void testlines(uint16_t color);
// void testdrawtext(char *text, uint16_t color);
// void testfastlines(uint16_t color1, uint16_t color2);
// void testdrawrects(uint16_t color);
// void testfillrects(uint16_t color1, uint16_t color2);
// void testfillcircles(uint8_t radius, uint16_t color);
// void testdrawcircles(uint8_t radius, uint16_t color);
// void testtriangles();
// void testroundrects();
// void tftPrintTest();
// void mediabuttons();

