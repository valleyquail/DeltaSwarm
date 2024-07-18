//
// Created by nikesh on 7/2/24.
//

#ifndef PICO_MAIN_SENSORS_CONFIG_H
#define PICO_MAIN_SENSORS_CONFIG_H

#include "../../include/config.h"
#include <Arduino.h>
#include <Wire.h>
#include "ICM42688.h"
#include "Adafruit_LIS3MDL.h"


struct imu_data_t {
    float accX;
    float accY;
    float accZ;
    float gyrX;
    float gyrY;
    float gyrZ;
    bool new_data;
};

struct mag_data_t {
    float magX;
    float magY;
    float magZ;
    bool new_data;
};

extern imu_data_t* imu_data;
extern mag_data_t* mag_data;

bool config_icm42688();

bool config_lis3mdl();

void read_sensors();


#endif //PICO_MAIN_SENSORS_CONFIG_H
