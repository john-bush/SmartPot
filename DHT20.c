#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"

#define SENSOR_ADDRESS 0x5C // I2C address of the sensor

void sensor_send_command(uint8_t command) {
    i2c_start();
    i2c_write(SENSOR_ADDRESS << 1);
    i2c_write(command);
    i2c_stop();
}

uint8_t sensor_read_data(uint8_t *buffer, uint8_t count) {
    uint8_t status;
    i2c_start();
    i2c_write((SENSOR_ADDRESS << 1) | 1);
    status = i2c_read(buffer, count);
    i2c_stop();
    return status;
}

int main(void) {
    uint8_t buffer[4];
    float temperature, humidity;
    i2c_init();
    while (1) {
        sensor_send_command(0x2C); // start measurement with high repeatability
        _delay_ms(16); // wait for measurement to complete
        if (sensor_read_data(buffer, 4) == 0) {
            humidity = ((float)buffer[0] << 8 | buffer[1]) / 10.0;
            temperature = ((float)(buffer[2] << 8 | buffer[3]) / 10.0) - 40.0;
            printf("Temperature: %.2f C\n", temperature);
            printf("Humidity: %.2f %%\n", humidity);
        } else {
            printf("I2C communication error, retrying...\n");
        }
        _delay_ms(2000); // wait 2 seconds before retrying
    }
    return 0;
}