//
// Created by nikesh on 7/2/24.
//

#include "sensors_config.h"
#include "../../lib/I2C_Control/i2c_control.h"
#include "calibration_params.h"

ICM42688 *imu;
Adafruit_LIS3MDL *mag;

imu_data_t* imu_data;
mag_data_t* mag_data;

bool config_icm42688() {
    imu = new ICM42688(Wire, ICM42688_ADDRESS);
    int status = imu->begin();
    if (status != 1) {
        Serial.printf("IMU not found\n");
        return false;
    }
    imu->setGyroFS(gyroFSR);
    imu->setAccelFS(accelFSR);
    imu->setAccelODR(odr);
    imu->setGyroODR(odr);
    imu->calibrateAccel();
    imu->calibrateGyro();

    return true;
}


//Magnetometer Configuration Parameters
bool config_lis3mdl() {
    mag = new Adafruit_LIS3MDL();
    mag->begin_I2C(LIS3MDL_ADDRESS, &Wire);
    if (!mag->begin_I2C(LIS3MDL_ADDRESS)) {
        Serial.printf("Magnetometer not found\n");
        return false;
    }
    mag->setPerformanceMode(performanceMode);
    mag->setOperationMode(operationMode);
    mag->setDataRate(dataRate);
    mag->setRange(readingsRange);
    //Disable all intertupts since this will be polled on a timer
    mag->configInterrupt(false, false, false, false, false, false);
    return true;
}


void read_sensors(){
// read the sensor

    imu->getAGT();

    // display the data
    Serial.printf("%f,", imu->accX());
    Serial.printf("%f,", imu->accY());
    Serial.printf("%f,", imu->accZ());
    Serial.printf("%f,", imu->gyrX());
    Serial.printf("%f,", imu->gyrY());
    Serial.printf("%f,", imu->gyrZ());


    float x,y,z;
    mag->readMagneticField(x,y,z);      // get X Y and Z data at once
    Serial.printf("%f, %f, %f\n", x, y, z);

}