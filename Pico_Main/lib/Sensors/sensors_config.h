//
// Created by nikesh on 7/2/24.
//

#ifndef PICO_MAIN_SENSORS_CONFIG_H
#define PICO_MAIN_SENSORS_CONFIG_H
#include <array>
#include "../../include/config.h"
#include <Arduino.h>
#include <Wire.h>

struct imu_data_t
{
    float accX;
    float accY;
    float accZ;
    float gyrX;
    float gyrY;
    float gyrZ;
    uint64_t timestamp;
};

struct mag_data_t
{
    float magX;
    float magY;
    float magZ;
    uint64_t timestamp;
};

struct optical_data_t{
    int16_t deltaX;
    int16_t deltaY;
    uint64_t timestamp;
};


extern imu_data_t imu_data_series[];
extern mag_data_t mag_data_series[];
extern optical_data_t optical_data_series[];

extern float acc_bias[];
extern float gyr_bias[];
extern float mag_bias[];

bool config_icm42688(std::array<float, 9> &calibration, bool manual = false);
int __calibrate_icm42688(std::array<float, 9> &calibration);
bool config_lis3mdl();

void read_sensors_debug();

#endif // PICO_MAIN_SENSORS_CONFIG_H
