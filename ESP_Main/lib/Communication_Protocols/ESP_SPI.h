#include <SPI/src/SPI.h>
#include <Arduino.h>

class SPI_Interface
{
public:
    SPI_Interface();
    void begin(); // Initialize the SPI bus
    void end();   // Disable the SPI bus
};