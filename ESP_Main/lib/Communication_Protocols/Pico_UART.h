#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

class Pico_UART
{
public:
    Pico_UART();
    void begin(int baudrate);
    void end();
    int available();
    int read();
    void write(uint8_t data);
    void flush();
    void setPins(int tx, int rx);
};