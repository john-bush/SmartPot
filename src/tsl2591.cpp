#include "tsl2591.h"
#include "i2c.h"
#include "interface.h"


void TSL2591::initialize(){
    enable();
    delay(40);
    set_timing_and_gain();
    delay(40);
    disable();
}

uint8_t TSL2591::enable(){
    uint8_t reg_addr[1];
    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE;
    
    uint8_t reg_data[1];

    reg_data[0] =  TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN |
             TSL2591_ENABLE_NPIEN;

    uint8_t res;

    res = i2c_io(LIGHT_SENSOR_ADDR, reg_addr, 1, reg_data, 1, nullptr, 0);
    return res;
}


uint8_t TSL2591::disable(){
    uint8_t reg_addr[1];
    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE;
    
    uint8_t reg_data[1];

    reg_data[0] =  TSL2591_ENABLE_POWEROFF;
    uint8_t res = 0;
    res = i2c_io(LIGHT_SENSOR_ADDR, reg_addr, 1, reg_data, 1, nullptr, 0);
    return res;
}

uint8_t TSL2591::set_timing_and_gain(){
    uint8_t reg_addr[1];
    uint8_t reg_data[1];

    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL;
    reg_data[0] = TSL2591_INTEGRATIONTIME_100MS | TSL2591_GAIN_MED;
    int res = i2c_io(LIGHT_SENSOR_ADDR, reg_addr, 1, reg_data , 1, nullptr, 0);
    return res;
}

long TSL2591::rd_luminosity(){
    uint8_t reg_addr[1];

    enable();

    delay(120);

    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW;
    uint8_t channel_one_t[2]; // Just the IR reading
    uint8_t channel_zero_t[2]; // Visible + IR
    i2c_io(LIGHT_SENSOR_ADDR, reg_addr,1, nullptr, 0, channel_zero_t, 2);

    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW;
    delay(120);

    i2c_io(LIGHT_SENSOR_ADDR, reg_addr,1, nullptr, 0, channel_one_t, 2);

    uint16_t channel_zero = ((uint16_t(channel_zero_t[0]) << 8) |  (uint16_t(channel_zero_t[1])));
    uint16_t channel_one = ((uint16_t(channel_one_t[0]) << 8) |  (uint16_t(channel_one_t[1])));


    delay(120);
    disable();
    
    return calculateLux(channel_zero, channel_one);
}


uint8_t TSL2591::rd_status(){
    uint8_t cmd[1];
    uint8_t result[1];
    cmd[0]= TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_STATUS;
    i2c_io(LIGHT_SENSOR_ADDR, cmd, 1, NULL, 0, result, 1);
    return result[0];
}


long TSL2591::calculateLux(uint16_t ch0, uint16_t ch1) {
  float atime, again;
  float cpl; 
  double lux;

  // Check for overflow conditions first
  if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF)) {
    // Signal an overflow
    return -1;
  }

  // Note: This algorithm is based on preliminary coefficients
  // provided by AMS and may need to be updated in the future

  atime = 100.0; //100ms
  again = 1.0; //no gain

  // cpl = (ATIME * AGAIN) / DF
  cpl =  (atime * again) / TSL2591_LUX_DF;

  // Original lux calculation (for reference sake)
  // float lux1 = ( (float)ch0 - (TSL2591_LUX_COEFB * (float)ch1) ) / cpl;
  // float lux2 = ( ( TSL2591_LUX_COEFC * (float)ch0 ) - ( TSL2591_LUX_COEFD *
  // (float)ch1 ) ) / cpl; lux = lux1 > lux2 ? lux1 : lux2;

  // Alternate lux calculation 1
  // See: https://github.com/adafruit/Adafruit_TSL2591_Library/issues/14
  //lux = (((float)ch0 - (float)ch1)) * (1.0F - ((float)ch1 / (float)ch0)) / cpl;

  // Alternate lux calculation 2
  lux = ( ch0 - 1.7*ch1) / cpl;
  
  // Signal I2C had no errors
  return long(lux);
}