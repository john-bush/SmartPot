#include <stdio.h>


#define DHT20_ADDRESS 0x70 // I2C address of the sensor from manufacturer

#define DHT20_TRIGGER_CMD 0xAC

#define DHT20_TRIGGER_DATA0 0x33
#define DHT20_TRIGGER_DATA1 0x00



class DHT20{

    public:
    
    uint8_t request_measurement();
    uint8_t read_sensors();

    uint8_t full_measurement();
    float get_temperature();
    float get_humidity();

    private:
    uint32_t temperature_raw, humidity_raw;

    float humidity;
    float temperature;

};
