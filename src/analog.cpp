#include "analog.h"
#include "interface.h"



int read_soil_moisture(){
  int soil_read = analogRead(SOIL_MOISTURE_PIN);
  // Do operations
  return soil_read;
}

int read_water_level_pins(){
  int pin1_val = analogRead(WATER_LEVEL_PIN_0);
  return pin1_val;

} 