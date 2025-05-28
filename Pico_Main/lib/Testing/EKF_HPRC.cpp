//#include "EKF.h"
//
//// TODO: Fix comments, do be wrong - Dan Pearson
//
///**
// * @brief Construct a new Quat State Estimator:: Quat State Estimator object
// *
// */
//StateEstimator::StateEstimator() {}
//
///**
// * @param initialOrientation
// * @param dt
// */
//void StateEstimator::init(BLA::Matrix<10> initialOrientation, float dt) {
//    this->x = initialOrientation;
//    this->x_min = initialOrientation;
//    this->dt = dt;
//    this->initialized = true;
//}
//
///**
// * @brief Run every loop of the state machine to perform the predict and update step of the EKF
// *
// * @param telemPacket Sensor Frame
// * @return BLA::Matrix<4> State Vector
// */
//void StateEstimator::onLoop(Utility::TelemPacket telemPacket)
//{
//    /* Read Data from Sensors and Convert to SI Units */
//
//    // Convert Accel values to m/s/s
//    float accX = telemPacket.accelX * g;
//    float accY = telemPacket.accelY * g;
//    float accZ = telemPacket.accelZ * g;
//
//    // Convert gyro values from deg/s to rad/s
//    float gyrX = telemPacket.gyroX * (PI / 180);
//    float gyrY = telemPacket.gyroY * (PI / 180);
//    float gyrZ = telemPacket.gyroZ * (PI / 180);
//
//    float magX = telemPacket.magX;
//    float magY = telemPacket.magY;
//    float magZ = telemPacket.magZ;
//
//    BLA::Matrix<6> u = {accX, accY, accZ, gyrX, gyrY, gyrZ};
//
//    // Apply measurement function to predict priori state of the system
//    x_min = measurementFunction(u);
//
//    // Take the jacobian to obtain the covariance of the prediction step
//    BLA::Matrix<10, 10> A = measurementJacobian(u);
//
//    // Update model covariance from previous state
//    BLA::Matrix<10, 6> W = updateModelCovariance(telemPacket);
//
//    // Apply updated model covariance to process noise covariance matrix
//    BLA::Matrix<10, 10> Q = W * gyroAccelVar * BLA::MatrixTranspose<BLA::Matrix<10, 6>>(W);
//
//    // Update Priori Error Covariance
//    P_min = A * P * BLA::MatrixTranspose<BLA::Matrix<10, 10>>(A) + Q;
//
//    BLA::Matrix<3> magVector = {magX, magY, magZ};
//    float magVectorLen = BLA::Norm(magVector);
//
//    BLA::Matrix<3> accelVector = {accX, accY, accZ};
//    float accelVectorLen = BLA::Norm(accelVector);
//
//    // Normalize Accel and Mag for use in correction step
//    if (magVectorLen != 0) {
//        magVector /= magVectorLen;
//    }
//    if (accelVectorLen != 0) {
//        accelVector /= accelVectorLen;
//    }
//
//    // Calculate update function with magnetometer readings to correct orientation
//    BLA::Matrix<6> z = {
//            accelVector(0), accelVector(1), accelVector(2), magVector(0), magVector(1), magVector(2)};
//
//    BLA::Matrix<6> h = updateFunction();
//
//    // Take the jacobian to obtain the covariance of the correction function
//    BLA::Matrix<6, 10> H = updateJacobian();
//
//    // Compute the kalman gain from the magnetometer covariance readings
//    BLA::Matrix<6> v = z - h;
//    BLA::Matrix<6, 6> S = H * P_min * BLA::MatrixTranspose<BLA::Matrix<6, 10>>(H) + R;
//    BLA::Matrix<10, 6> K = P_min * BLA::MatrixTranspose<BLA::Matrix<6, 10>>(H) * BLA::Inverse(S);
//
//    // Use our kalman gain and magnetometer readings to correct priori orientation
//    x = x_min + K * v;
//
//    // Update error covariance matrix
//    P = (eye10 - K * H) * P_min;
//
//    // Serial.println("<----- State ----->");
//    // for (int i = 0; i < x.Rows; i++) {
//    //     for (int j = 0; j < x.Cols; j++) {
//    //         Serial.print(String(x(i,j)) + "\t");
//    //     }
//    //     Serial.println("");
//    // }
//
//    float quatNorm = sqrt(x(0) * x(0) + x(1) * x(1) + x(2) * x(2) + x(3) * x(3));
//    if (quatNorm != 0) {
//        x(0) = x(0) / quatNorm;
//        x(1) = x(1) / quatNorm;
//        x(2) = x(2) / quatNorm;
//        x(3) = x(3) / quatNorm;
//    }
//
//    // Serial.print("VEL|");
//    // Serial.print(x(7)); Serial.print(",");
//    // Serial.print(x(8)); Serial.print(",");
//    // Serial.println(x(9));
//
//    // float r_adj = Utility::r_earth + telemPacket.gpsAltMSL; // [m]
//    // float N_earth = Utility::a_earth / sqrt(1 - pow(Utility::e_earth,2) * pow(sin(telemPacket.gpsLat), 2));
//
//    // float X_NEW = (N_earth + telemPacket.gpsAltAGL) * cos(telemPacket.gpsLat * DEG_TO_RAD) * cos(telemPacket.gpsLong * DEG_TO_RAD);
//    // float Y_NEW = (N_earth + telemPacket.gpsAltAGL) * cos(telemPacket.gpsLat * DEG_TO_RAD) * sin(telemPacket.gpsLong * DEG_TO_RAD);
//    // // float Z_0 = (((Utility::b_earth*Utility::b_earth)/(Utility::a_earth*Utility::a_earth))*N_earth + telemPacket.gpsAltAGL) * sin(telemPacket.gpsLat * DEG_TO_RAD);
//    // float Z_NEW = (N_earth*(1-pow(Utility::e_earth,2))+telemPacket.gpsAltAGL)*sin(telemPacket.gpsLat);
//
//    // x(4) = X_NEW;
//    // x(5) = Y_NEW;
//    // x(6) = Z_NEW;
//}
//
//BLA::Matrix<10> StateEstimator::measurementFunction(BLA::Matrix<6> u)
//{
//    float p = u(3);
//    float q = u(4);
//    float r = u(5);
//    BLA::Matrix<3> accelR = {u(0), u(1), u(2)}; // Accel Readings
//
//    float qw = x(0);
//    float qx = x(1);
//    float qy = x(2);
//    float qz = x(3);
//
//    BLA::Matrix<4> f_q = {
//            qw - (dt/2.0f)*p*qx - (dt/2.0f)*q*qy - (dt/2.0f)*r*qz,
//            qx + (dt/2.0f)*p*qw - (dt/2.0f)*q*qz - (dt/2.0f)*r*qy,
//            qy + (dt/2.0f)*p*qz + (dt/2.0f)*q*qw - (dt/2.0f)*r*qx,
//            qz - (dt/2.0f)*p*qy + (dt/2.0f)*q*qx - (dt/2.0f)*r*qw
//    };
//    float f_qLen = BLA::Norm(f_q);
//
//    // Calculate linear accelerations in the NED Frame
//    // BLA::Matrix<4> quat = {x(0), x(1), x(2), x(3)};
//
//    // BLA::Matrix<3, 3> R_BT = quat2rotm(quat);
//
//    // Rotate Acceleration into NED
//
//    // BLA::Matrix<3> accelT = BLA::MatrixTranspose<BLA::Matrix<3,3>>(R_BT) * accelR;
//
//    // Apply Gravity Compensation
//    // BLA::Matrix<3> gravNED = {0, 0, -g};
//    // BLA::Matrix<3> gravB = BLA::MatrixTranspose<BLA::Matrix<3, 3>>(R_BT) * gravNED; // Gravity in body frame
//    // BLA::Matrix<3> accelB = accelR - gravB;                                         // Linear Accelartions in body frame
//
//    // BLA::Matrix<3> accelT = BLA::MatrixTranspose<BLA::Matrix<3, 3>>(R_BT) * accelB;
//
//    // Serial.println("<----- Linear Accel ----->");
//    // for (int i = 0; i < accelT.Rows; i++) {
//    //     for (int j = 0; j < accelT.Cols; j++) {
//    //         Serial.print(String(accelT(i,j)) + "\t");
//    //     }
//    //     Serial.println("");
//    // }
//
//    // Normalize Quaternion Derivative Function
//    // if (f_qLen != 0) {
//    //     f_q /= f_qLen;
//    // }
//
//    // BLA::Matrix<3> f_p = {
//    //     x(4) + (x(7)*dt + 0.5*accelT(0)*(dt*dt)),
//    //     x(5) + (x(8)*dt + 0.5*accelT(1)*(dt*dt)),
//    //     x(6) + (x(9)*dt + 0.5*accelT(2)*(dt*dt))
//    // };
//
//    // BLA::Matrix<3> f_v = {
//    //     x(7) + accelT(0)*dt,
//    //     x(8) + accelT(1)*dt,
//    //     x(9) + accelT(2)*dt
//    // };
//
//    BLA::Matrix<3> f_v = {0, 0, 0};
//    BLA::Matrix<3> f_p = {0, 0, 0};
//
//    BLA::Matrix<10> f = {
//            f_q(0),
//            f_q(1),
//            f_q(2),
//            f_q(3),
//            f_v(0),
//            f_v(1),
//            f_v(2),
//            f_p(0),
//            f_p(1),
//            f_p(2)};
//
//    return f;
//};
//
//BLA::Matrix<10, 10> StateEstimator::measurementJacobian(BLA::Matrix<6> u)
//{
//    float p = u(3);
//    float q = u(4);
//    float r = u(5);
//
//    BLA::Matrix<10, 10> A = {
//            1, -(dt/2.0f)*p, -(dt/2.0f)*q, -(dt/2.0f)*r, 0,0,0,0,0,0,
//            (dt/2.0f)*p, 1, (dt/2.0f)*r, -(dt/2.0f)*q, 0,0,0,0,0,0,
//            (dt/2.0f)*q, -(dt/2.0f)*r, 1, (dt/2.0f)*p, 0,0,0,0,0,0,
//            (dt/2.0f)*r, (dt/2.0f)*q, -(dt/2.0f)*p, 1, 0,0,0,0,0,0,
//            0,0,0,0,0,0,0,0,0,0,
//            0,0,0,0,0,0,0,0,0,0,
//            0,0,0,0,0,0,0,0,0,0,
//            0,0,0,0,0,0,0,0,0,0,
//            0,0,0,0,0,0,0,0,0,0,
//            0,0,0,0,0,0,0,0,0,0,
//    };
//
//    return A;
//}
//
//BLA::Matrix<6> StateEstimator::updateFunction()
//{
//    BLA::Matrix<3> r = {
//            cos(magneticDip), 0, sin(magneticDip)
//    };
//    float rLen = BLA::Norm(r);
//
//    // Must normalize vector for corrections step
//    if (rLen != 0) {
//        r /= rLen;
//    }
//
//    BLA::Matrix<3> G = {0, 0, -g}; // NED Gravity Vector
//
//    // Must normalize vector for correction step
//    G = G / BLA::Norm(G);
//
//    // Normalize quaternion prior to calculation
//    BLA::Matrix<4> q = {x_min(0), x_min(1), x_min(2), x_min(3)};
//    float qLen = BLA::Norm(q);
//    if (qLen != 0) {
//        q /= qLen;
//    }
//
//    BLA::Matrix<3, 3> quatRotm = quat2rotm(q);
//
//    BLA::Matrix<3> a_hat = BLA::MatrixTranspose<BLA::Matrix<3, 3>>(quatRotm) * G;
//
//    BLA::Matrix<3> m_hat = BLA::MatrixTranspose<BLA::Matrix<3, 3>>(quatRotm) * r;
//
//    BLA::Matrix<6> h = {
//            a_hat(0),
//            a_hat(1),
//            a_hat(2),
//            m_hat(0),
//            m_hat(1),
//            m_hat(2)
//    };
//
//    return h;
//};
//
//BLA::Matrix<6, 10> StateEstimator::updateJacobian()
//{
//    BLA::Matrix<3> r = {
//            cos(magneticDip), 0, sin(magneticDip)
//    }; // NED Mag vector
//    float rLen = BLA::Norm(r);
//
//    // Must normalize vector for corrections step
//    if (rLen != 0) {
//        r /= rLen;
//    }
//
//    BLA::Matrix<3> G = {0, 0, -g}; // NED Gravity Vector
//
//    // Gravity Unit Vector for orientation step
//    G = G / BLA::Norm(G);
//
//    // Normalize quaternion prior to calculation
//    BLA::Matrix<4> q = {x_min(0), x_min(1), x_min(2), x_min(3)};
//    float qLen = BLA::Norm(q);
//    if (qLen != 0) {
//        q /= qLen;
//    }
//
//    float rx = r(0);
//    float ry = r(1);
//    float rz = r(2);
//    float gx = G(0);
//    float gy = G(1);
//    float gz = G(2);
//
//    float qw = q(0);
//    float qx = q(1);
//    float qy = q(2);
//    float qz = q(3);
//
//    BLA::Matrix<6, 10> H = {
//            gx*qw+gy*qz-gz*qy, gx*qx+gy*qy+gz*qz, -gx*qy+gy*qx-gz*qw, -gx*qz+gy*qw+gz*qx, 0,0,0,0,0,0,
//            -gx*qz+gy*qw+gz*qx, gx*qy-gy*qx+gz*qw, gx*qx+gy*qy+gz*qz, -gx*qw-gy*qz+gz*qy, 0,0,0,0,0,0,
//            gx*qy-gy*qx+gz*qw, gx*qz-gy*qw-gz*qx, gx*qw+gy*qz-gz*qy, gx*qx+gy*qy+gz*qz, 0,0,0,0,0,0,
//            rx*qw+ry*qz-rz*qy, rx*qy+ry*qy+rz*qz, -rx*qy+ry*qx-rz*qw, -rx*qz+ry*qw+rz*qx, 0,0,0,0,0,0,
//            -rx*qz+ry*qw+rz*qx, rx*qy-ry*qx+rz*qw, rx*qx+ry*qy+rz*qz, -rx*qw-ry*qz+rz*qy, 0,0,0,0,0,0,
//            rx*qy-ry*qx+rz*qw, rx*qz-ry*qw-rz*qx, rx*qw+ry*qz-rz*qy, rx*qx+ry*qy+rz*qz, 0,0,0,0,0,0
//    };
//
//    return H;
//};
//
//BLA::Matrix<10, 6> StateEstimator::updateModelCovariance(Utility::TelemPacket    telemPacket)
//{
//
//    BLA::Matrix<10, 6> W = {
//            -x(1), -x(2), -x(3), 0, 0, 0,
//            x(0), -x(3), x(2), 0, 0, 0,
//            x(3), x(0), -x(1), 0, 0, 0,
//            -x(2), x(1), x(0), 0, 0, 0,
//            0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0,
//    };
//
//    return W * (dt / 2.0f);
//};
//
//BLA::Matrix<3, 3> StateEstimator::quat2rotm(BLA::Matrix<4> q)
//{
//
//    float qw = q(0);
//    float qx = q(1);
//    float qy = q(2);
//    float qz = q(3);
//
//    BLA::Matrix<3, 3> rotm = {
//            qw*qw + qx*qx - qy*qy - qz*qz, 2 * (qx * qy - qw * qz), 2 * (qx * qz + qw * qy),
//            2 * (qx * qy + qw * qz), qw*qw - qx*qx + qy*qy - qz*qz, 2 * (qy * qz - qw * qx),
//            2 * (qx * qz - qw * qy), 2 * (qw * qx + qy * qz), qw*qw - qx*qx - qy*qy + qz*qz
//    };
//
//    return rotm;
//};
//
//BLA::Matrix<4> StateEstimator::quaternionMultiplication(BLA::Matrix<4> q1, BLA::Matrix<4> q2)
//{
//
//    float w1 = q1(0);
//    float i1 = q1(1);
//    float j1 = q1(2);
//    float k1 = q1(3);
//
//    float w2 = q2(0);
//    float i2 = q2(1);
//    float j2 = q2(2);
//    float k2 = q2(3);
//
//    BLA::Matrix<4> res;
//
//    res(0) = w1 * w2 - i1 * i2 - j1 * j2 - k1 * k2;
//    res(1) = w1 * i2 + i1 * w2 + j1 * k2 - k1 * j2;
//    res(2) = w1 * j2 - i1 * k2 + j1 * w2 + k1 * i2;
//    res(3) = w1 * k2 + i1 * j2 - j1 * i2 + k1 * w2;
//
//    return res;
//};