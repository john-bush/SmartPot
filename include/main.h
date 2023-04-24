#pragma once

#include "util.h"
#include "tft_display_lib.h"
#include "interface.h"
#include <string.h>
// #include <Adafruit_GFX.h>    // Core graphics library
// #include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
// #include <SPI.h>


/**************************************************************************/
/**
 * Constants and Declarations for the Encoder
*/
// Define pin numbers for encoder
#define BUTTON_PIN PIND2
#define ENCODER_PIN_A PIND3
#define ENCODER_PIN_B PIND4
// Define variables for encoder state and button state
volatile int encoderState = 0;
volatile int lastEncoderState = 0;
volatile int buttonState = 0;
volatile int lastButtonState = 0;
volatile bool buttonPressed = false;

// Define variable for and encoder count
volatile int encoderCount = 0;

// Define debounce variables
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void encoderISR();
void buttonISR();
void InitEncoder();

void PollSensors();

void RetrievePastState();

char* PlantCare();

// persistent state variables
bool configured = false; // true if the user has configured the system
int state = 0; // general state variable
bool firstLoop = true; // true if this is the first loop in that state
int plantType = -1; // the type of plant that the user has selected (is -1 if not configured)

// sensor variables
uint32_t luminosity = 0;
float humidity = 0;
float temperature = 0;
int soilMoisture = 0;
bool waterTank = false;
bool fertilizerTank = false;


// plant specific variables
float idealHumidity = 0;
float idealTemperature = 0;
int idealLuminosity = 0;
int idealSoilMoisture = 0;

#define DT 0.1
// plant care integral variables
char* response1;
char* response2;
char* response3;
float previousHumidity = 0;
float previousTemperature = 0;
int previousLuminosity = 0;
int previousSoilMoisture = 0;

float humidityIntegral = 0;
float temperatureIntegral = 0;
int luminosityIntegral = 0;
int soilMoistureIntegral = 0;


// thresholds for plant care
int soilMoistureThreshold = 40;
int humidityThreshold = 100;
int temperatureThreshold = 100;
int luminosityThreshold = 1000;

// watering variables
bool watering = false;  
bool fertilizing = false;
int amountOfWater = 2000;
unsigned int wateringTimeout = 10000;
unsigned long fertilizerTimeout = 100000;
unsigned long startTime = 0;
unsigned long lastWater = 0;
unsigned long lastFertilizer = 0;
unsigned long curr_time = 0;

int plantLight = 0;


/**************************************************************************/