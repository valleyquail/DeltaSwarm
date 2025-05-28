//
// Created by nikesh on 7/16/24.
//

#ifndef PICO_MAIN_EKF_H
#define PICO_MAIN_EKF_H
#include "sensors_config.h"
#include <ArduinoEigenDense.h>

class EKF
{

protected:
    // These ere used as pointers to the data structures in the sensors_config.h file
    // It needs to be done this way to prevent duplicates of the structures and to ensure atomicity in the system
    imu_data_t *imu_data;
    mag_data_t *mag_data;

    // State estimate (quaternion representation)
    // Eigen::Matrix<float, 4> state_estimate;

public:
    EKF(imu_data_t *imu_data, mag_data_t *mag_data) : imu_data(imu_data), mag_data(mag_data) {}

    // Function to initialize the EKF
    void init();

    // Function to update the EKF with new sensor data
    void update();

    // Function to get the current state estimate
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> *getStateEstimate();

     // Function to get the current covariance estimate
     Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> *getCovarianceEstimate();

     // Function to reset the EKF
     void reset();
};

#endif // PICO_MAIN_EKF_H
