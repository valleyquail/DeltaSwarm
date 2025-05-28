//
// Created by nikesh on 7/2/24.
//

#ifndef PICO_MAIN_SENSORS_CONFIG_H
#define PICO_MAIN_SENSORS_CONFIG_H

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
    bool new_data;
};

struct mag_data_t
{
    float magX;
    float magY;
    float magZ;
    bool new_data;
};

extern imu_data_t *imu_data;
extern mag_data_t *mag_data;
extern float acc_bias[];
extern float gyr_bias[];
extern float mag_bias[];

bool config_icm42688(float accel_calibration[6], float gyro_calibration[3]);
float *__calibrate_icm42688();

bool config_lis3mdl();

void read_sensors_debug();

#endif // PICO_MAIN_SENSORS_CONFIG_H
