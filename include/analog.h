// Definition for all of the Analog Sensors
// Include the 2 water level sensors + the soil moisture sensor
#include "stdio.h"

#define SOIL_MOISTURE_PIN PC0
#define WATER_LEVEL_PIN_0 PC1
#define WATER_LEVEL_PIN_1 PC2
#define WATER_PUMP_PIN PIND5
#define FERTILIZER_PUMP_PIN PIND6



int read_soil_moisture();
int read_water_level_pins();


void init_pumps();
void turn_on_water_pump();
void turn_off_water_pump();
void turn_on_fertilizer_pump();
void turn_off_fertilizer_pump();



