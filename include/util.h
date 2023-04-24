#pragma once

#include <EEPROM.h>

void writeIntIntoEEPROM(int address, int number);

int readIntFromEEPROM(int address, int& number);

void writeBoolIntoEEPROM(int address, bool val);

void readBoolFromEEPROM(int address, bool& val);