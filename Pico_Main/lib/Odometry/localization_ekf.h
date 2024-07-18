//
// Created by nikesh on 7/16/24.
//

#ifndef PICO_MAIN_LOCALIZATION_EKF_H
#define PICO_MAIN_LOCALIZATION_EKF_H

#include "EKF.h"
/**
 * @brief Class for the Extended Kalman Filter for localization
 * This is to be used by default when the robot is on the ground to keep track of its position and orientation
 * Fuses data from encoders, IMU, and magnetometer to estimate the position and orientation of the robot
 */
class localization_ekf : public EKF {

};


#endif //PICO_MAIN_LOCALIZATION_EKF_H
