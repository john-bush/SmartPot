#include "tsl2591.h"
#include "i2c.h"
#include "interface.h"



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

uint32_t TSL2591::rd_luminosity(){
    uint32_t result;

    enable();

    delay(100);

    uint8_t reg_addr[1];
    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW;
    
    uint8_t channel_one[2]; // Just the IR reading
    uint8_t channel_zero[2]; // Visible + IR

    i2c_io(LIGHT_SENSOR_ADDR, reg_addr,1, nullptr, 0, channel_zero, 2);
    
    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW;

    delay(100);

    uint8_t s = i2c_io(LIGHT_SENSOR_ADDR, reg_addr,1, nullptr, 0, channel_one, 2);

    result = ((uint32_t(channel_zero[0]) << 8) | 
             (uint32_t(channel_zero[1]))) -
             ((uint32_t(channel_one[0]) << 8 ) | 
             (uint32_t(channel_one[1])));

    delay(100);
    disable();
    
    return result;
}


uint8_t TSL2591::rd_status(){
    uint8_t cmd[1];
    uint8_t result[1];
    cmd[0]= TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_STATUS;
    i2c_io(LIGHT_SENSOR_ADDR, cmd, 1, NULL, 0, result, 1);
    return result[0];
}