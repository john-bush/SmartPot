#include "analog.h"
#include "Arduino.h"


void read_soil_moisture(int& soil_moisture){
    int raw_read = analogRead(SOIL_MOISTURE_PIN);
    // Do operations
    
    soil_moisture = raw_read;
}

void read_tank_levels(int& pin1_val, int& pin2_val){
    pin1_val = analogRead(WATER_LEVEL_PIN_0);
    pin2_val = analogRead(WATER_LEVEL_PIN_1);
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
