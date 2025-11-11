#include "Calibration.h"
#include <EEPROM.h>

#include "sensors_config.h"
#include "motor.h"
#include "motion_controller.h"

void calibrate_imu()
{
    // This function is used to calibrate the IMU
    // It will read the accelerometer and gyroscope data and calculate the bias
    // The bias will be stored in the acc_bias and gyr_bias arrays

    // Uncomment the following lines to enable calibration
    std::array<float, 9> calibration;
    __calibrate_icm42688(calibration);
    EEPROM.put(IMU_CALIBRATION_ADDRESS, calibration.data());
}

void calibrate_encoders()
{
    // This function is used to calibrate the encoders
    // It will read the encoder data and calculate the bias
    // The bias will be stored in the encoder_calibration array
    int* motor_calibration = MotionController::runMotorCalibration();
    EEPROM.put(ENCODER_CALIBRATION_ADDRESS, motor_calibration);
}