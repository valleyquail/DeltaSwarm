//
// Created by nikesh on 7/16/24.
//

#include "EKF.h"



BLA::Matrix<3, 3> EKF::quat2rotm(BLA::Matrix<4> q)
{

    float qw = q(0);
    float qx = q(1);
    float qy = q(2);
    float qz = q(3);

    BLA::Matrix<3, 3> rotm = {
            qw*qw + qx*qx - qy*qy - qz*qz, 2 * (qx * qy - qw * qz), 2 * (qx * qz + qw * qy),
            2 * (qx * qy + qw * qz), qw*qw - qx*qx + qy*qy - qz*qz, 2 * (qy * qz - qw * qx),
            2 * (qx * qz - qw * qy), 2 * (qw * qx + qy * qz), qw*qw - qx*qx - qy*qy + qz*qz
    };

    return rotm;
};

BLA::Matrix<4> EKF::quaternionMultiplication(BLA::Matrix<4> q1, BLA::Matrix<4> q2)
{

    float w1 = q1(0);
    float i1 = q1(1);
    float j1 = q1(2);
    float k1 = q1(3);

    float w2 = q2(0);
    float i2 = q2(1);
    float j2 = q2(2);
    float k2 = q2(3);

    BLA::Matrix<4> res;

    res(0) = w1 * w2 - i1 * i2 - j1 * j2 - k1 * k2;
    res(1) = w1 * i2 + i1 * w2 + j1 * k2 - k1 * j2;
    res(2) = w1 * j2 - i1 * k2 + j1 * w2 + k1 * i2;
    res(3) = w1 * k2 + i1 * j2 - j1 * i2 + k1 * w2;

    return res;
};