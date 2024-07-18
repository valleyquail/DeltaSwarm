//
// Created by nikesh on 7/16/24.
//

#ifndef PICO_MAIN_ORIENTATION_EKF_H
#define PICO_MAIN_ORIENTATION_EKF_H

#include "EKF.h"

/**
 * @brief Class for the Extended Kalman Filter for orientation
 * This is a secondary EKF that will be used when the robot is picked up
 * Postional data is useless since it can be calculated from the position of the robots picking it up
 * So all the data that is needed is the orientation of the robot using the IMU and magnetometer with
 * quaternion representation since now the robot can rotate in 3D space and not just 2D
 */

class orientation_ekf : public EKF {

};


#endif //PICO_MAIN_ORIENTATION_EKF_H
