#include "analog.h"
#include "Arduino.h"


int read_soil_moisture(){
  int soil_read = analogRead(SOIL_MOISTURE_PIN);
  // Do operations
  return soil_read;
}

int read_water_level_pins(){
  int pin1_val = analogRead(WATER_LEVEL_PIN_0);
  return pin1_val;

} 


// Water and fertilizer Pumps

void init_pumps(){
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(FERTILIZER_PUMP_PIN, OUTPUT);
}

void turn_on_water_pump(){
  digitalWrite(WATER_PUMP_PIN, HIGH);
}

void turn_off_water_pump(){
  digitalWrite(WATER_PUMP_PIN, LOW);
}

void turn_on_fertilizer_pump(){
  digitalWrite(FERTILIZER_PUMP_PIN, HIGH);
}

void turn_off_fertilizer_pump(){
  digitalWrite(FERTILIZER_PUMP_PIN, LOW);
}
