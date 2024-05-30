//
// Created by nikesh on 5/24/24.
//

#ifndef PICO_MAIN_ICM_42688_REGISTERS_H
#define PICO_MAIN_ICM_42688_REGISTERS_H
#include "pico/stdlib.h"

#pragma once

/*
 * ICM-42688 structure
*/

typedef struct {

    int16_t Accel_X;
    int16_t Accel_Y;
    int16_t Accel_Z;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X;
    int16_t Gyro_Y;
    int16_t Gyro_Z;
    double Gx;
    double Gy;
    double Gz;

} ICM42688_t;

/*
 * ICM-42688 Registers
*/


const uint8_t REG_BANK_SEL = 0x76; //R/W
// ------------------ Bank 0 ------------------
const uint8_t DEVICE_CONFIG =  0x11; //R/W
const uint8_t DRIVE_CONFIG =  0x13; //R/W
const uint8_t INT_CONFIG =  0x14; //
const uint8_t FIFO_CONFIG =  0x16; //R/W
const uint8_t TEMP_DATA1 =  0x1D; //SYNCR
const uint8_t TEMP_DATA0 =  0x1E; //SYNCR
const uint8_t ACCEL_DATA_X1 =  0x1F; //SYNCR ACCEL_DATA_X[15:8]
const uint8_t ACCEL_DATA_X0 =  0x20; //SYNCR ACCEL_DATA_X[7:0]
const uint8_t ACCEL_DATA_Y1 =  0x21; //SYNCR ACCEL_DATA_Y[15:8
const uint8_t ACCEL_DATA_Y0 =  0x22; //SYNCR ACCEL_DATA_Y[7:0]
const uint8_t ACCEL_DATA_Z1 =  0x23; //SYNCR ACCEL_DATA_Z[15:8]
const uint8_t ACCEL_DATA_Z0 =  0x24; //SYNCR ACCEL_DATA_Z[15:8]

const uint8_t GYRO_DATA_X1 =  0x25; //SYNCR GYRO_DATA_X[15:8]
const uint8_t GYRO_DATA_X0 =  0x26; //SYNCR GYRO _DATA_X[7:0]
const uint8_t GYRO_DATA_Y1 =  0x27; //SYNCR GYRO _DATA_X[7:0]
const uint8_t GYRO_DATA_Y0 =  0x28; //SYNCR GYRO _DATA_X[7:0]
const uint8_t GYRO_DATA_Z1 =  0x29; //SYNCR GYRO_DATA_Z[15:8]
const uint8_t GYRO_DATA_Z0 =  0x2A; //SYNCR GYRO_DATA_Z[15:8]

const uint8_t TMST_FSYNCH =  0x2B; //SYNCR
const uint8_t TMST_FSYNCL =  0x2C; //SYNCR
const uint8_t INT_STATUS =  0x2D; //R/C
const uint8_t FIFO_COUNTH =  0x2E; //R
const uint8_t FIFO_COUNTL =  0x2F; //R
const uint8_t FIFO_DATA =  0x30; //R
const uint8_t APEX_DATA0 =  0x31; //SYNCR
const uint8_t APEX_DATA1 =  0x32; //SYNCR
const uint8_t APEX_DATA2 =  0x33; //R
const uint8_t APEX_DATA3 =  0x34; //R
const uint8_t APEX_DATA4 =  0x35; //R
const uint8_t APEX_DATA5 =  0x36; //R
const uint8_t INT_STATUS2 =  0x37; //R/C
const uint8_t INT_STATUS3 =  0x38; //R/C
const uint8_t SIGNAL_PATH_RESET =  0x4B; //W/C
const uint8_t INTF_CONFIG0 =  0x4C; //R/W
const uint8_t INTF_CONFIG1 =  0x4D; //R/W
const uint8_t PWR_MGMT0 =  0x4E; //R/W
const uint8_t GYRO_CONFIG0 =  0x4F; //R/W
const uint8_t ACCEL_CONFIG0 =  0x50; //R/W
const uint8_t GYRO_CONFIG1 =  0x51; //R/W
const uint8_t GYRO_ACCEL_CONFIG0 =  0x52; //R/W
const uint8_t ACCEL_CONFIG1 =  0x53; //R/W
const uint8_t TMST_CONFIG =  0x54; //R/W
const uint8_t APEX_CONFIG0 =  0x56; //R/W
const uint8_t SMD_CONFIG =  0x57; //R/W
const uint8_t FIFO_CONFIG1 =  0x5F; //R/W
const uint8_t FIFO_CONFIG2 =  0x60; //R/W
const uint8_t FIFO_CONFIG3 =  0x61; //R/W
const uint8_t FSYNC_CONFIG =  0x62; //R/W
const uint8_t INT_CONFIG0 =  0x63; //R/W
const uint8_t INT_CONFIG1 =  0x64; //R/W
const uint8_t INT_SOURCE0 =  0x65; //R/W
const uint8_t INT_SOURCE1 =  0x66; //R/W
const uint8_t INT_SOURCE3 =  0x68; //R/W
const uint8_t INT_SOURCE4 =  0x69; //R/W
const uint8_t FIFO_LOST_PKT0 =  0x6C; //R
const uint8_t FIFO_LOST_PKT1 =  0x6D; //R
const uint8_t SELF_TEST_CONFIG =  0x70; //R/W
const uint8_t WHO_AM_I =  0x75; //R
// ------------------ Bank 1 ------------------
const uint8_t SENSOR_CONFIG0 =  0x03; //R/W
const uint8_t GYRO_CONFIG_STATIC2 =  0x0B; //R/W
const uint8_t GYRO_CONFIG_STATIC3 =  0x0C; //R/W
const uint8_t GYRO_CONFIG_STATIC4 =  0x0D; //R/W
const uint8_t GYRO_CONFIG_STATIC5 =  0x0E; //R/W
const uint8_t GYRO_CONFIG_STATIC6 =  0x0F; //R/W
const uint8_t GYRO_CONFIG_STATIC7 =  0x10; //R/W
const uint8_t GYRO_CONFIG_STATIC8 =  0x11; //R/W
const uint8_t GYRO_CONFIG_STATIC9 =  0x12; //R/W
const uint8_t GYRO_CONFIG_STATIC10 =  0x13; //R/W
const uint8_t XG_ST_DATA =  0x5F; //R/W
const uint8_t YG_ST_DATA =  0x60; //R/W
const uint8_t ZG_ST_DATA =  0x61; //R/W
const uint8_t TMSTVAL0 =  0x62; //R
const uint8_t TMSTVAL1 =  0x63; //R
const uint8_t TMSTVAL2 =  0x64; //R
const uint8_t INTF_CONFIG4 =  0x7A; //R/W
const uint8_t INTF_CONFIG5 =  0x7B; //R/W
const uint8_t INTF_CONFIG6 =  0x7C; //R/W
// ------------------ Bank 2 ------------------
const uint8_t ACCEL_CONFIG_STATIC2 =  0x03; //R/W
const uint8_t ACCEL_CONFIG_STATIC3 =  0x04; //R/W
const uint8_t ACCEL_CONFIG_STATIC4 =  0x05; //R/W
const uint8_t XA_ST_DATA =  0x3B; //R/W
const uint8_t YA_ST_DATA =  0x3C; //R/W
const uint8_t ZA_ST_DATA =  0x3D; //R/W
// ------------------ Bank 4 ------------------
const uint8_t APEX_CONFIG1 =  0x40; //R/W
const uint8_t APEX_CONFIG2 =  0x41; //R/W
const uint8_t APEX_CONFIG3 =  0x42; //R/W
const uint8_t APEX_CONFIG4 =  0x43; //R/W
const uint8_t APEX_CONFIG5 =  0x44; //R/W
const uint8_t APEX_CONFIG6 =  0x45; //R/W
const uint8_t APEX_CONFIG7 =  0x46; //R/W
const uint8_t APEX_CONFIG8 =  0x47; //R/W
const uint8_t APEX_CONFIG9 =  0x48; //R/W
const uint8_t ACCEL_WOM_X_THR =  0x4A; //R/W
const uint8_t ACCEL_WOM_Y_THR =  0x4B; //R/W
const uint8_t ACCEL_WOM_Z_THR =  0x4C; //R/W
const uint8_t INT_SOURCE6 =  0x4D; //R/W
const uint8_t INT_SOURCE7 =  0x4E; //R/W
const uint8_t INT_SOURCE8 =  0x4F; //R/W
const uint8_t INT_SOURCE9 =  0x50; //R/W
const uint8_t INT_SOURCE10 =  0x51; //R/W
const uint8_t OFFSET_USER0 =  0x77; //R/W
const uint8_t OFFSET_USER1 =  0x78; //R/W
const uint8_t OFFSET_USER2 =  0x79; //R/W
const uint8_t OFFSET_USER3 =  0x7A; //R/W
const uint8_t OFFSET_USER4 =  0x7B; //R/W
const uint8_t OFFSET_USER5 =  0x7C; //R/W
const uint8_t OFFSET_USER6 =  0x7D; //R/W
const uint8_t OFFSET_USER7 =  0x7E; //R/W
const uint8_t OFFSET_USER8 =  0x7F; //R/W

#endif //PICO_MAIN_ICM_42688_REGISTERS_H
