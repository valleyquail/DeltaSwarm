//
// Created by nikesh on 7/2/24.
//

#ifndef PICO_MAIN_CALIBRATION_PARAMS_H
#define PICO_MAIN_CALIBRATION_PARAMS_H

#include <Arduino.h>
#include "ICM42688.h"
#include "Adafruit_LIS3MDL.h"
//IMU Configuration Parameters
//Change these as needed based on the desired behavior of the robot
// 500dps max
const ICM42688::GyroFS gyroFSR = ICM42688::dps500;
// 4g max
const ICM42688::AccelFS accelFSR = ICM42688::gpm4;
//Data Rate Output
//1000Hz max
const ICM42688::ODR odr = ICM42688::odr1k;

//Digital Motion Processor Configuration
//const ICM42688::DMPConfig dmpConfig = ICM42688::DMP_ENABLED;

//Magnetometer Configuration Parameters
//Change these as needed based on the desired behavior of the robot
// 4 gauss max
const lis3mdl_range_t readingsRange = LIS3MDL_RANGE_4_GAUSS;
// 80Hz max
const lis3mdl_dataRate_t dataRate = LIS3MDL_DATARATE_80_HZ;
// Ultra High Performance Mode
const lis3mdl_performancemode_t performanceMode = LIS3MDL_ULTRAHIGHMODE;
// Operation Mode
const lis3mdl_operationmode_t operationMode = LIS3MDL_CONTINUOUSMODE;







#endif //PICO_MAIN_CALIBRATION_PARAMS_H
