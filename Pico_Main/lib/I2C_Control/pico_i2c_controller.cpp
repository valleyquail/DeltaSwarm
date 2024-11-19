//
// Created by nikesh on 5/24/24.
//
#include <Arduino.h>
#include <Wire.h>
#include "../../include/config.h"
#include "i2c_control.h"
#include "hardware/i2c.h"


void initPicoController() {
    Wire1.setSCL(SCL_PIN_1);
    Wire1.setSDA(SDA_PIN_1);
    Serial.println("huh");
    Wire1.setClock(100000);
    Serial.println("huh");
    Wire1.begin();

    Serial.printf("initialized the pico as a master\n");
}



//Port testing debug

bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void bus_scan() {
    Serial.printf("\nI2C Bus Scan\n");
    Serial.printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            Serial.printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = 1;
        else

            ret = i2c_read_blocking(i2c1, addr, &rxdata, 1, false);

        Serial.printf(ret < 0 ? "." : "@");
        Serial.printf(addr % 16 == 15 ? "\n" : "  ");
    }
    Serial.printf("Done.\n");
}