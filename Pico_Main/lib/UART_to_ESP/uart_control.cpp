#include "uart_control.h"

void encoder_message(int *encoder_values)
{
    sprintf(send_buffer, "Encoder values: %d, %d, %d", encoder_values[0], encoder_values[1], encoder_values[2]);
}
