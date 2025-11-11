//=====================================================================================================
// MadgwickAHRS.c
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
// 19/02/2012	SOH Madgwick	Magnetometer measurement is normalised
//
//=====================================================================================================


/**
 * @brief Class for the Madgwick filter for orientation
 * This is a Madgwick filter that will be used for calculating the orientation of the robot in 3D space
 * Since the odometry alone will not provide accurate orientation information,
 * especially since the optical flow sensor only provides 2D movement data and
 * is susceptible to orientation changes.
 */


#include "MadgwickAHRS.h"
#include <pico/float.h>


uint8_t MadgwickAHRS::update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
	float recipNorm;
	float qDot1, qDot2, qDot3, qDot4;
	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
		updateIMU(gx, gy, gz, ax, ay, az);
		return 1;
	}

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-qx * gx - qy * gy - qz * gz);
	qDot2 = 0.5f * (qw * gx + qy * gz - qz * gy);
	qDot3 = 0.5f * (qw * gy - qx * gz + qz * gx);
	qDot4 = 0.5f * (qw * gz + qx * gy - qy * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
		float s0, s1, s2, s3;
		float hx, hy;
		float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

		// Normalise accelerometer measurement
		recipNorm = fastInvSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;   

		// Normalise magnetometer measurement
		recipNorm = fastInvSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * qw * mx;
		_2q0my = 2.0f * qw * my;
		_2q0mz = 2.0f * qw * mz;
		_2q1mx = 2.0f * qx * mx;
		_2q0 = 2.0f * qw;
		_2q1 = 2.0f * qx;
		_2q2 = 2.0f * qy;
		_2q3 = 2.0f * qz;
		_2q0q2 = 2.0f * qw * qy;
		_2q2q3 = 2.0f * qy * qz;
		q0q0 = qw * qw;
		q0q1 = qw * qx;
		q0q2 = qw * qy;
		q0q3 = qw * qz;
		q1q1 = qx * qx;
		q1q2 = qx * qy;
		q1q3 = qx * qz;
		q2q2 = qy * qy;
		q2q3 = qy * qz;
		q3q3 = qz * qz;

		// Reference direction of Earth's magnetic field
		hx = mx * q0q0 - _2q0my * qz + _2q0mz * qy + mx * q1q1 + _2q1 * my * qy + _2q1 * mz * qz - mx * q2q2 - mx * q3q3;
		hy = _2q0mx * qz + my * q0q0 - _2q0mz * qx + _2q1mx * qy - my * q1q1 + my * q2q2 + _2q2 * mz * qz - my * q3q3;
		_2bx = sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * qy + _2q0my * qx + mz * q0q0 + _2q1mx * qz - mz * q1q1 + _2q2 * my * qz - mz * q2q2 + mz * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * qy * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * qz + _2bz * qx) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * qy * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * qx * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * qz * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * qy + _2bz * qw) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * qz - _4bz * qx) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * qy * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * qy - _2bz * qw) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * qx + _2bz * qz) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * qw - _4bz * qy) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * qz + _2bz * qx) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * qw + _2bz * qy) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * qx * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		recipNorm = fastInvSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	qw += qDot1 * (1.0f / sampleFreq);
	qx += qDot2 * (1.0f / sampleFreq);
	qy += qDot3 * (1.0f / sampleFreq);
	qz += qDot4 * (1.0f / sampleFreq);

	// Normalise quaternion
	recipNorm = fastInvSqrt(qw * qw + qx * qx + qy * qy + qz * qz);
	qw *= recipNorm;
	qx *= recipNorm;
	qy *= recipNorm;
	qz *= recipNorm;
	return 2;
}

//---------------------------------------------------------------------------------------------------
// IMU algorithm update

void MadgwickAHRS::updateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;

	// Rate of change of quaternion from gyroscope
	float qDot1 = 0.5f * (-qx * gx - qy * gy - qz * gz);
	float qDot2 = 0.5f * (qw * gx + qy * gz - qz * gy);
	float qDot3 = 0.5f * (qw * gy - qx * gz + qz * gx);
	float qDot4 = 0.5f * (qw * gz + qx * gy - qy * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = fastInvSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;   

		// Auxiliary variables to avoid repeated arithmetic
		const float _2q0 = 2.0f * qw;
		const float _2q1 = 2.0f * qx;
		const float _2q2 = 2.0f * qy;
		const float _2q3 = 2.0f * qz;
		const float _4q0 = 4.0f * qw;
		const float _4q1 = 4.0f * qx;
		const float _4q2 = 4.0f * qy;
		const float _8q1 = 8.0f * qx;
		const float _8q2 = 8.0f * qy;
		const float q0q0 = qw * qw;
		const float q1q1 = qx * qx;
		const float q2q2 = qy * qy;
		const float q3q3 = qz * qz;

		// Gradient decent algorithm corrective step
		float s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		float s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * qx - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		float s2 = 4.0f * q0q0 * qy + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		float s3 = 4.0f * q1q1 * qz - _2q1 * ax + 4.0f * q2q2 * qz - _2q2 * ay;
		recipNorm = fastInvSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	qw += qDot1 * (1.0f / sampleFreq);
	qx += qDot2 * (1.0f / sampleFreq);
	qy += qDot3 * (1.0f / sampleFreq);
	qz += qDot4 * (1.0f / sampleFreq);

	// Normalise quaternion
	recipNorm = fastInvSqrt(qw * qw + qx * qx + qy * qy + qz * qz);
	qw *= recipNorm;
	qx *= recipNorm;
	qy *= recipNorm;
	qz *= recipNorm;
}

etl::array<float, 3> MadgwickAHRS::getRPY() const {

	// roll (x-axis rotation)
	float sinr_cosp = 2 * (qw * qx + qy * qz);
	float cosr_cosp = 1 - 2 * (qx * qx + qy * qy);
	float roll = atan2f(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = sqrtf(1 + 2 * (qw * qy - qx * qz));
	float cosp = sqrtf(1 - 2 * (qw * qy - qx * qz));
	float pitch = 2 * atan2f(sinp, cosp) - M_PI / 2;

	// yaw (z-axis rotation)
	float siny_cosp = 2 * (qw * qz + qx * qy);
	float cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
	float yaw = atan2f(siny_cosp, cosy_cosp);

	return {roll, pitch, yaw};
}

float MadgwickAHRS::getYaw() const {
	// yaw (z-axis rotation)
	float siny_cosp = 2 * (qw * qz + qx * qy);
	float cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
	return atan2f(siny_cosp, cosy_cosp);

}

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float MadgwickAHRS::fastInvSqrt(const float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *reinterpret_cast<long *>(&y);
	i = 0x5f3759df - (i>>1);
	y = *reinterpret_cast<float *>(&i);
	y = y * (1.5f - (halfx * y * y));
	return y;
}
//=====================================================================================================