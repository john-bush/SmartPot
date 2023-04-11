#include "tsl2591.h"
#include "i2c.h"



uint8_t tsl_enable(){
    uint8_t reg_addr[1];
    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE;
    
    uint8_t reg_data[1];

    reg_data[0] =  TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN |
             TSL2591_ENABLE_NPIEN;

    uint8_t res;

    res = i2c_io(LIGHT_SENSOR_ADDR, reg_addr, 1, reg_data, 1, nullptr, 0);
    return res;
}


uint8_t tsl_disable(){
    uint8_t reg_addr[1];
    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE;
    
    uint8_t reg_data[1];

    reg_data[0] =  TSL2591_ENABLE_POWEROFF;
    uint8_t res;
    res = i2c_io(LIGHT_SENSOR_ADDR, reg_addr, 1, reg_data, 1, nullptr, 0);
    return res;
}

uint32_t rd_tsl_luminosity(){

    tsl_enable();
    _delay_ms(120);

    uint32_t result;

    uint8_t reg_addr[1];
    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW;
    
    uint8_t rd_data_h[2];
    uint8_t rd_data_l[2];

    i2c_io(LIGHT_SENSOR_ADDR, reg_addr,1, nullptr, 0, rd_data_l, 2);
    
    reg_addr[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_HIGH;

    i2c_io(LIGHT_SENSOR_ADDR, reg_addr,1, nullptr, 0, rd_data_h, 2);

    result = rd_data_l[0];
    
    result = (static_cast<uint32_t>(rd_data_h[0]) << 24 ) | 
             (static_cast<uint32_t>(rd_data_h[1]) << 16 ) | 
             (static_cast<uint32_t>(rd_data_l[0]) << 8 ) | 
             (static_cast<uint32_t>(rd_data_l[1]));

    tsl_disable();


    return result;

}


uint8_t rd_tsl_status(){
    uint8_t io_status;
    uint8_t cmd[1];
    uint8_t result[1];
    cmd[0]= TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_STATUS;
    io_status = i2c_io(LIGHT_SENSOR_ADDR, cmd, 1, NULL, 0, result, 1);
    return result[0];
}