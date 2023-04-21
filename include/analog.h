// Definition for all of the Analog Sensors
// Include the 2 water level sensors + the soil moisture sensor
#include "stdio.h"

#define SOIL_MOISTURE_PIN PC0
#define WATER_LEVEL_PIN_0 PC1
#define WATER_LEVEL_PIN_1 PC2


int read_soil_moisture();
int read_water_level_pins();

