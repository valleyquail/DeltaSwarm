#pragma once
#include <pico/stdlib.h>


constexpr uint8_t ENCODER_CALIBRATION_ADDRESS = 0x00; // Stores 4 floats per motor
constexpr uint8_t IMU_CALIBRATION_ADDRESS = 0x12; // Stores 3 floats for accelerometer bias, 3 floats for scale bias, and
                                                  // 3 floats for gyroscope bias,
constexpr uint8_t MAGNETOMETER_CALIBRATION_ADDRESS = IMU_CALIBRATION_ADDRESS + 0x09;

void calibrate_imu();
void calibrate_magnetometer();
void calibrate_encoders();



