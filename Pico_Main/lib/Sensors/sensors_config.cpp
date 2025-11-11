// Created by nikesh on 7/2/24.
//
#include <stdlib.h>
#include "sensors_config.h"
#include "../../lib/I2C_Control/i2c_control.h"
#include "calibration_params.h"
#include "ICM42688.h"
#include "Adafruit_LIS3MDL.h"


imu_data_t *imu_data;
mag_data_t *mag_data;
float acc_bias[3];
float gyr_bias[3];
float mag_bias[3];

ICM42688 imu = ICM42688(Wire, ICM42688_ADDRESS);
Adafruit_LIS3MDL mag = Adafruit_LIS3MDL();

inline int __calibrate_icm42688(std::array<float, 9> &calibration)
{
    // Reset the IMU to default values
    imu.setAccelCalX(0.0f, 1.0f);
    imu.setAccelCalY(0.0f, 1.0f);
    imu.setAccelCalZ(0.0f, 1.0f);
    imu.setGyroBiasX(0.0f);
    imu.setGyroBiasY(0.0f);
    imu.setGyroBiasZ(0.0f);
    // This function is used to calibrate the IMU
    // It will read the accelerometer and gyroscope data and calculate the bias
    // The bias will be stored in the acc_bias and gyr_bias arrays
    int err;
    err = imu.calibrateAccel();
    if (err < 0)
    {
        return -1;
    }
    err = imu.calibrateGyro();

    if (err < 0)
    {
        return -2;
    }

    acc_bias[0] = imu.getAccelBiasX_mss();
    acc_bias[1] = imu.getAccelBiasY_mss();
    acc_bias[2] = imu.getAccelBiasZ_mss();
    gyr_bias[0] = imu.getGyroBiasX();
    gyr_bias[1] = imu.getGyroBiasY();
    gyr_bias[2] = imu.getGyroBiasZ();

    calibration[0] = acc_bias[0];
    calibration[1] = imu.getAccelScaleFactorX();
    calibration[2] = acc_bias[1];
    calibration[3] = imu.getAccelScaleFactorY();
    calibration[4] = acc_bias[2];
    calibration[5] = imu.getAccelScaleFactorZ();
    calibration[6] = gyr_bias[0];
    calibration[7] = gyr_bias[1];
    calibration[8] = gyr_bias[2];

    return 1;
}

bool config_icm42688(std::array<float, 9> &calibration, bool manual)
{
    int status = imu.begin();
    if (status != 1)
    {
        Serial.printf("IMU not found\n");
        return false;
    }
    Serial.printf("IMU Found\n");

    if (!manual)
    {
        status = __calibrate_icm42688(calibration);
        if (status < 0)
        {
            Serial.printf("IMU Calibration failed with error code %d\n", status);
            return false;
        }
        Serial.printf("IMU Calibrated\n");
    }

    imu.setGyroFS(gyroFSR);
    imu.setAccelFS(accelFSR);
    imu.setAccelODR(odr);
    imu.setGyroODR(odr);

    imu.setAccelCalX(calibration[0], calibration[1]);
    imu.setAccelCalY(calibration[2], calibration[3]);
    imu.setAccelCalZ(calibration[4], calibration[5]);
    imu.setGyroBiasX(calibration[6]);
    imu.setGyroBiasY(calibration[7]);
    imu.setGyroBiasZ(calibration[8]);

    return true;
}


//Magnetometer Configuration Parameters
bool config_lis3mdl() {

    mag.begin_I2C(LIS3MDL_ADDRESS, &Wire);
    if (!mag.begin_I2C(LIS3MDL_ADDRESS)) {
        Serial.printf("Magnetometer not found\n");
        return false;
    }
    Serial.printf("Magnetometer Found\n");
    mag.setPerformanceMode(performanceMode);
    mag.setOperationMode(operationMode);
    mag.setDataRate(dataRate);
    mag.setRange(readingsRange);
    //Disable all intertupts since this will be polled on a timer
    mag.configInterrupt(false, false, false, false, false, false);
    return true;
}

void read_sensors_debug()
{
    // read the sensor

    imu.getAGT();

    // display the data
    Serial.printf("%f,", imu.accX());
    Serial.printf("%f,", imu.accY());
    Serial.printf("%f,", imu.accZ());
    Serial.printf("%f,", imu.gyrX());
    Serial.printf("%f,", imu.gyrY());
    Serial.printf("%f,", imu.gyrZ());

    float x, y, z;
    mag.readMagneticField(x, y, z); // get X Y and Z data at once
    Serial.printf("%f, %f, %f\n", x, y, z);
}
