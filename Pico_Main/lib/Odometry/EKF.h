//
// Created by nikesh on 7/16/24.
//

#ifndef PICO_MAIN_EKF_H
#define PICO_MAIN_EKF_H
#include "../../include/BasicLinearAlgebra/BasicLinearAlgebra.h"
#include "sensors_config.h"
class EKF {

protected:
    //These ere used as pointers to the data structures in the sensors_config.h file
    //It needs to be done this way to prevent duplicates of the structures and to ensure atomicity in the system
    imu_data_t* imu_data;
    mag_data_t* mag_data;


public:



BLA::Matrix<3, 3> quat2rotm(BLA::Matrix<4> q);
BLA::Matrix<4> quaternionMultiplication(BLA::Matrix<4> q1, BLA::Matrix<4> q2);


};


#endif //PICO_MAIN_EKF_H
