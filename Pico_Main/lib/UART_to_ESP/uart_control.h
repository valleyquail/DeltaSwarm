#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

char data_buffer[100];

char send_buffer[100];

void encoder_message(int *encoder_values);

/**
 * @brief Class that handles the UART communication with the ESP32
 *
 */

class UARTController
{
private:
    int rxPin;
    int txPin;
    int baudRate;

public:
    UARTController(int rxPin, int txPin, int baudRate);
    void init();
    void send(char *data);
    char *receive();
};