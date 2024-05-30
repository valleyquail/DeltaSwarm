//
// Created by nikesh on 5/25/24.
//

#ifndef PICO_MAIN_LIS3MDL_REGISTERS_H
#define PICO_MAIN_LIS3MDL_REGISTERS_H

#include "pico/stdlib.h"

/*=========================================================================
I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
#define LIS3MDL_I2CADDR_DEFAULT (0x1C) ///< Default breakout addres
/*=========================================================================*/

#define LIS3MDL_REG_WHO_AM_I 0x0F  ///< Register that contains the part ID
#define LIS3MDL_REG_CTRL_REG1 0x20 ///< Register address for control 1
#define LIS3MDL_REG_CTRL_REG2 0x21 ///< Register address for control 2
#define LIS3MDL_REG_CTRL_REG3 0x22 ///< Register address for control 3
#define LIS3MDL_REG_CTRL_REG4 0x23 ///< Register address for control 3
#define LIS3MDL_REG_STATUS 0x27    ///< Register address for status
#define LIS3MDL_REG_OUT_X_L 0x28   ///< Register address for X axis lower byte
#define LIS3MDL_REG_INT_CFG 0x30   ///< Interrupt configuration register
#define LIS3MDL_REG_INT_THS_L 0x32 ///< Low byte of the irq threshold

/** The magnetometer ranges */
typedef enum {
    LIS3MDL_RANGE_4_GAUSS = 0b00,  ///< +/- 4g (default value)
    LIS3MDL_RANGE_8_GAUSS = 0b01,  ///< +/- 8g
    LIS3MDL_RANGE_12_GAUSS = 0b10, ///< +/- 12g
    LIS3MDL_RANGE_16_GAUSS = 0b11, ///< +/- 16g
} lis3mdl_range_t;

/** The magnetometer data rate, includes FAST_ODR bit */
typedef enum {
    LIS3MDL_DATARATE_0_625_HZ = 0b0000, ///<  0.625 Hz
    LIS3MDL_DATARATE_1_25_HZ = 0b0010,  ///<  1.25 Hz
    LIS3MDL_DATARATE_2_5_HZ = 0b0100,   ///<  2.5 Hz
    LIS3MDL_DATARATE_5_HZ = 0b0110,     ///<  5 Hz
    LIS3MDL_DATARATE_10_HZ = 0b1000,    ///<  10 Hz
    LIS3MDL_DATARATE_20_HZ = 0b1010,    ///<  20 Hz
    LIS3MDL_DATARATE_40_HZ = 0b1100,    ///<  40 Hz
    LIS3MDL_DATARATE_80_HZ = 0b1110,    ///<  80 Hz
    LIS3MDL_DATARATE_155_HZ = 0b0001,   ///<  155 Hz (FAST_ODR + UHP)
    LIS3MDL_DATARATE_300_HZ = 0b0011,   ///<  300 Hz (FAST_ODR + HP)
    LIS3MDL_DATARATE_560_HZ = 0b0101,   ///<  560 Hz (FAST_ODR + MP)
    LIS3MDL_DATARATE_1000_HZ = 0b0111,  ///<  1000 Hz (FAST_ODR + LP)
} lis3mdl_dataRate_t;

/** The magnetometer performance mode */
typedef enum {
    LIS3MDL_LOWPOWERMODE = 0b00,  ///< Low power mode
    LIS3MDL_MEDIUMMODE = 0b01,    ///< Medium performance mode
    LIS3MDL_HIGHMODE = 0b10,      ///< High performance mode
    LIS3MDL_ULTRAHIGHMODE = 0b11, ///< Ultra-high performance mode
} lis3mdl_performancemode_t;

/** The magnetometer operation mode */
typedef enum {
    LIS3MDL_CONTINUOUSMODE = 0b00, ///< Continuous conversion
    LIS3MDL_SINGLEMODE = 0b01,     ///< Single-shot conversion
    LIS3MDL_POWERDOWNMODE = 0b11,  ///< Powered-down mode
} lis3mdl;


#endif //PICO_MAIN_LIS3MDL_REGISTERS_H
