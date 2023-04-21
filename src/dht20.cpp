#include "dht20.h"
#include "i2c.h"
#include "interface.h"


uint8_t DHT20::request_measurement(){
    uint8_t reg_cmd[1];
    uint8_t reg_data[2];
    uint8_t status;


    reg_cmd[0] = DHT20_TRIGGER_CMD;
    reg_data[0] = DHT20_TRIGGER_DATA0;
    reg_data[1] = DHT20_TRIGGER_DATA1;

    status = i2c_io(DHT20_ADDRESS, reg_cmd  , 1, reg_data , 2 , NULL , 0);
    return status;
}

uint8_t DHT20::read_sensors(){

    uint8_t  result [7];
    uint8_t reg_cmd[1];
    uint8_t status;

    reg_cmd[0] = 0x71; // Todo figure out

    status = i2c_io(DHT20_ADDRESS, reg_cmd, 1, nullptr, 0 ,result, 7);

    humidity_raw =  (uint32_t(result[3] & 0xF0 ) >> 4) | (uint32_t(result[1]) << 12)  | (uint32_t(result[2]) << 4);
    temperature_raw =  (uint32_t(result[3] & 0x0F) << 16) | (uint32_t(result[4]) << 8)  | uint32_t(result[5]);

    humidity =  float(humidity_raw) / 0x100000 * 100;
    temperature = float(temperature_raw)/ 5242.88 -50;
    
    return status;
}


float DHT20::get_temperature(){
    return temperature;
}


float DHT20::get_humidity(){
    return humidity;
}

uint8_t DHT20::full_measurement(){
    uint8_t res;
    request_measurement();
    delay(100);
    res = read_sensors();

    return res;

}




 


