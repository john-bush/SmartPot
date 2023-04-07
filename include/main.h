#pragma once

#include "tft_display_lib.h"
#include "interface.h"
// #include <Adafruit_GFX.h>    // Core graphics library
// #include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
// #include <SPI.h>


void LoadScreen();



/**************************************************************************/
/**
 * Constants and Declarations for the Encoder
*/
// Define pin numbers for encoder
const int encoderPinA = 2;
const int encoderPinB = 3;
const int buttonPin = 4;

// Define variables for encoder state and button state
volatile int encoderState = 0;
volatile int lastEncoderState = 0;
volatile int buttonState = 0;
volatile int lastButtonState = 0;

// Define variable for and encoder count
volatile int encoderCount = 0;

// Define debounce variables
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void encoderISR();
void buttonISR();
void InitEncoder();

/**************************************************************************/