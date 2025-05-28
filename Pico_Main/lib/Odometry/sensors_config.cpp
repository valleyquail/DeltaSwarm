//
// Created by nikesh on 7/2/24.
//

#include "sensors_config.h"
#include "../../lib/I2C_Control/i2c_control.h"
#include "calibration_params.h"
#include "ICM42688.h"
#include "Adafruit_LIS3MDL.h"

ICM42688 *imu;
Adafruit_LIS3MDL *mag;

imu_data_t *imu_data;
mag_data_t *mag_data;
float acc_bias[3];
float gyr_bias[3];
float mag_bias[3];

ICM42688 imu = ICM42688(Wire, ICM42688_ADDRESS);
Adafruit_LIS3MDL mag = Adafruit_LIS3MDL();

bool config_icm42688(float accel_calibration[6], float gyro_calibration[3])
{
    int status = imu->begin();
    if (status != 1)
    {
        Serial.printf("IMU not found\n");
        return false;
    }
    Serial.printf("IMU Found\n");
    imu->setGyroFS(gyroFSR);
    imu->setAccelFS(accelFSR);
    imu->setAccelODR(odr);
    imu->setGyroODR(odr);

    imu->setAccelCalX(accel_calibration[0], accel_calibration[1]);
    imu->setAccelCalY(accel_calibration[2], accel_calibration[3]);
    imu->setAccelCalZ(accel_calibration[4], accel_calibration[5]);
    imu->setGyroBiasX(gyro_calibration[0]);
    imu->setGyroBiasY(gyro_calibration[1]);
    imu->setGyroBiasZ(gyro_calibration[2]);

    return true;
}

float *__calibrate_icm42688()
{
    // Reset the IMU to default values
    imu->setAccelCalX(0.0f, 1.0f);
    imu->setAccelCalY(0.0f, 1.0f);
    imu->setAccelCalZ(0.0f, 1.0f);
    imu->setGyroBiasX(0.0f);
    imu->setGyroBiasY(0.0f);
    imu->setGyroBiasZ(0.0f);
    // This function is used to calibrate the IMU
    // It will read the accelerometer and gyroscope data and calculate the bias
    // The bias will be stored in the acc_bias and gyr_bias arrays

    imu->calibrateAccel();
    imu->calibrateGyro();
    float acc_gyro_bias[9];
    acc_gyro_bias[0] = imu->getAccelBiasX_mss();
    acc_gyro_bias[1] = imu->getAccelScaleFactorX();
    acc_gyro_bias[2] = imu->getAccelBiasY_mss();
    acc_gyro_bias[3] = imu->getAccelScaleFactorY();
    acc_gyro_bias[4] = imu->getAccelBiasZ_mss();
    acc_gyro_bias[5] = imu->getAccelScaleFactorZ();
    acc_gyro_bias[0] = imu->getGyroBiasX();
    acc_gyro_bias[1] = imu->getGyroBiasY();
    acc_gyro_bias[2] = imu->getGyroBiasZ();

    return acc_gyro_bias;
}

//Magnetometer Configuration Parameters
bool config_lis3mdl() {

    mag->begin_I2C(LIS3MDL_ADDRESS, &Wire);
    if (!mag->begin_I2C(LIS3MDL_ADDRESS)) {
        Serial.printf("Magnetometer not found\n");
        return false;
    }
    Serial.printf("Magnetometer Found\n");
    mag->setPerformanceMode(performanceMode);
    mag->setOperationMode(operationMode);
    mag->setDataRate(dataRate);
    mag->setRange(readingsRange);
    //Disable all intertupts since this will be polled on a timer
    mag->configInterrupt(false, false, false, false, false, false);
    return true;
}

void read_sensors_debug()
{
    // read the sensor

    imu->getAGT();

    // display the data
    Serial.printf("%f,", imu->accX());
    Serial.printf("%f,", imu->accY());
    Serial.printf("%f,", imu->accZ());
    Serial.printf("%f,", imu->gyrX());
    Serial.printf("%f,", imu->gyrY());
    Serial.printf("%f,", imu->gyrZ());

    float x, y, z;
    mag->readMagneticField(x, y, z); // get X Y and Z data at once
    Serial.printf("%f, %f, %f\n", x, y, z);
}
