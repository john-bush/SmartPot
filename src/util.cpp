#include "util.h"

void writeIntIntoEEPROM(int address, int number)
{ 
    EEPROM.write(address, number >> 8);
    EEPROM.write(address + 1, number & 0xFF);
}
int readIntFromEEPROM(int address, int& number)
{
    number = (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

void writeBoolIntoEEPROM(int address, bool val)
{
    EEPROM.write(address, val);
}

void readBoolFromEEPROM(int address, bool& val)
{
    val = EEPROM.read(address);
}