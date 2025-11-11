//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
// 11/10/2025   Nikesh Walling  Modified for C++ and project integration
//=====================================================================================================

#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h


#include <etl/array.h>
#include <ArduinoEigen.h>

class MadgwickAHRS {
    const float sampleFreq = 512.0f;		// sample frequency in Hz
    float beta = 0.1f;				// algorithm gain
    // Avoid using Eigen Quaternion since the code is already heavily optimized
    float qw = 1.0f, qx = 0.0f, qy = 0.0f, qz = 0.0f;	// quaternion of sensor frame relative to auxiliary frame
    static float fastInvSqrt(float x);

public:
    uint8_t update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    void updateIMU(float gx, float gy, float gz, float ax, float ay, float az);

    etl::array<float, 4> getQuaternion() const {
        return {qw, qx, qy, qz};
    }

     Eigen::Quaternion<float> getEigenQuaternion() const {
        return Eigen::Quaternion(qw, qx, qy, qz);
    }


    etl::array<float,3> getRPY() const;

    float getYaw() const;


};

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
