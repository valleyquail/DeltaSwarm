#include <Arduino.h>
#include "../../include/pin_definitions.h"
#include "../../include/config.h"
#include "i2c_control.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "../Motion/motion_controller.h"
extern MotionController motionController;

/**
 * Code referenced from this forum post:
 * https://forums.raspberrypi.com/viewtopic.php?t=304074
 *
 */
uint8_t packet_address;
struct DataPacket data_packets[NUM_PICO_REGISTERS];
// Interrupt handler for when the ESP requests data from the Pico
//  Handles which piece of information to be sent back to the ESP
static void __not_in_flash_func(i2c0_irq_handler)()
{
    uint32_t status = i2c0->hw->intr_stat;
#ifdef DEBUG
    Serial.printf("Interrupt status: %x\n", status);
#endif
    // Check to see if we have received data from the I2C controller
    if (status & I2C_IC_INTR_STAT_R_RX_FULL_BITS)
    {
        // Read the data (this will clear the interrupt)
        uint32_t value = i2c0->hw->data_cmd;

        // Check if this is the 1st byte we have received
        if (value & I2C_IC_DATA_CMD_FIRST_DATA_BYTE_BITS)
        {
            // If so treat it as the address to use
            packet_address = (uint8_t)(value & I2C_IC_DATA_CMD_DAT_BITS);
            data_packets[packet_address].status.message_sent = false;
            data_packets[packet_address].index = 0;
#ifdef DEBUG
            Serial.printf("Received address: %i\n", packet_address);
#endif
        }
        else
        {
            // If not 1st byte then store the data in the RAM
            // and increment the address to point to next byte
            data_packets[packet_address].buffer[data_packets[packet_address].index] = (uint8_t)(value & I2C_IC_DATA_CMD_DAT_BITS);
            data_packets[packet_address].index++;
        }
    }

    // Check to see if the I2C controller is requesting data
    if (status & I2C_IC_INTR_STAT_R_RD_REQ_BITS)
    {

        // Write the data from the current address in RAM
        i2c0->hw->data_cmd = (uint32_t)data_packets[packet_address].buffer[data_packets[packet_address].index];

        // Clear the interrupt
        i2c0->hw->clr_rd_req;

        // Increment the address
        data_packets[packet_address].index++;
    }
    if (status & I2C_IC_INTR_STAT_R_TX_ABRT_BITS)
    {
        i2c0->hw->clr_tx_abrt;
        data_packets[packet_address].status.message_sent = true;
#ifdef DEBUG
        Serial.printf("TX abort detected\n");
#endif
    }
    if (status & I2C_IC_INTR_STAT_R_STOP_DET_BITS)
    {
        i2c0->hw->clr_stop_det;
        data_packets[packet_address].status.message_sent = true;
#ifdef DEBUG
        Serial.printf("Stop detected\n");
#endif
        if (packet_address == MOTOR_SPEEDS)
        {
#ifdef DEBUG
            Serial.printf("Motor speeds: %c, %c\n", data_packets[MOTOR_SPEEDS].buffer[0], data_packets[MOTOR_SPEEDS].buffer[1]);
#endif
            motionController.setSpeedFromI2C(data_packets[MOTOR_SPEEDS].buffer);
        }
    }
}

void initPicoPeriph()
{

    // Set up the device as a peripheral
    i2c_init(i2c0, PICO_I2C_FREQ);
    i2c_set_slave_mode(i2c0, true, PICO_ADDRESS);
    // Set the GPIO pin functions
    gpio_init(SDA_PIN_1);
    gpio_init(SCL_PIN_1);
    gpio_set_function(SDA_PIN_1, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN_1);
    gpio_set_function(SCL_PIN_1, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN_1);

    // Enable the I2C interrupts we want to process
    i2c0->hw->intr_mask = I2C_IC_INTR_MASK_M_RX_FULL_BITS | I2C_IC_INTR_MASK_M_RD_REQ_BITS | I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS | I2C_IC_INTR_MASK_M_STOP_DET_BITS;

    // Set up the interrupt handler to service I2C interrupts
    irq_set_exclusive_handler(I2C0_IRQ, i2c0_irq_handler);

    // Enable I2C interrupt
    irq_set_enabled(I2C0_IRQ, true);
    Serial.printf("initialized the pico as a peripheral at address 0x%x\n", PICO_ADDRESS);
    data_packets[MOTOR_SPEEDS].buffer[0] = 'O';
    data_packets[MOTOR_SPEEDS].buffer[1] = 'K';
}

void initPicoController()
{
    gpio_init(SDA_PIN_0);
    gpio_set_function(SDA_PIN_0, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN_0);

    gpio_init(SCL_PIN_0);
    gpio_set_function(SCL_PIN_0, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN_0);

    i2c_init(i2c1, PICO_I2C_FREQ);
    Serial.printf("initialized the pico as a master\n");
}
