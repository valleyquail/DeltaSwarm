#include <Arduino.h>
#include "../../include/config.h"
#include "i2c_control.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "../Motion/motion_controller.h"

//Hashmap of function pointers to call when a packet is received
std::unordered_map<int, i2c_response_t> i2cFunctions;
/*
 * This is a hashmap that maps the packet address to the index of the data packet so that the addresses in the Pico
 * registers do not have to be consecutive
 */
std::unordered_map<uint8_t, int8_t> address_map;
int8_t data_packet_access_assignment_index = 0;

// Function to register a function to be called when a packet is received by adding it to a hashmap
void register_i2c_function(i2c_response_t function, int8_t packet_address) {
    i2cFunctions[packet_address] = function;
    address_map[packet_address] = (data_packet_access_assignment_index++);
}

// Function to call the function registered to a packet address
inline void i2cReponse(int packet_address) {
    if (i2cFunctions.find(packet_address) != i2cFunctions.end()) {
        int8_t packet_index = address_map[packet_address];
        i2cFunctions[packet_address](packet_index);
    }
}


uint8_t packet_address;
int8_t packet_index;
struct DataPacket data_packets[NUM_PICO_REGISTERS];
/**
 * Code referenced from this forum post:
 * https://forums.raspberrypi.com/viewtopic.php?t=304074
 *
 */
// Interrupt handler for when the ESP requests data from the Pico
//  Handles which piece of information to be sent back to the ESP
static void __not_in_flash_func(i2c0_irq_handler)() {
    uint32_t status = i2c0->hw->intr_stat;
#ifdef I2C_DEBUG
    Serial.printf("Interrupt status: %lx\n", status);
#endif
    // Check to see if we have received data from the I2C controller
    if (status & I2C_IC_INTR_STAT_R_RX_FULL_BITS) {
        // Read the data (this will clear the interrupt)
        uint32_t value = i2c0->hw->data_cmd;

        // Check if this is the 1st byte we have received
        if (value & I2C_IC_DATA_CMD_FIRST_DATA_BYTE_BITS) {
            // If so treat it as the address to use
            packet_address = (uint8_t) (value & I2C_IC_DATA_CMD_DAT_BITS);
            //retrieves the actual array index of the data packet
            packet_index = address_map[packet_address];
            data_packets[packet_index].status.message_sent = false;
            data_packets[packet_index].index = 0;
#ifdef I2C_DEBUG
            Serial.printf("Received address: %i\n", packet_address);
#endif
        } else {
            // If not 1st byte then store the data in the buffer
            // and increment the address to point to next byte
#ifdef I2C_DEBUG
            uint8_t old_data = data_packets[packet_index].buffer[data_packets[packet_index].index];
                        data_packets[packet_index].buffer[data_packets[packet_index].index] = (uint8_t) (status &
                                                                                                         I2C_IC_DATA_CMD_DAT_BITS);
                        Serial.printf("Old data: %i\n", old_data);
                        Serial.printf("Received data: %i\n", data_packets[packet_index].buffer[data_packets[packet_index].index]);
#else
            data_packets[packet_index].buffer[data_packets[packet_index].index] = (uint8_t) (status &
                                                                                             I2C_IC_DATA_CMD_DAT_BITS);
#endif
            data_packets[packet_index].index++;
        }
    }

    // Check to see if the I2C controller is requesting data
    if (status & I2C_IC_INTR_STAT_R_RD_REQ_BITS) {

        // Write the data from the current address in RAM
        i2c0->hw->data_cmd = (uint32_t) data_packets[packet_index].buffer[data_packets[packet_index].index];

#ifdef I2C_DEBUG
        Serial.printf("Sending data: %i\n", data_packets[packet_index].buffer[data_packets[packet_index].index]);
#endif
        data_packets[packet_index].index++;
        // Clear the interrupt
        i2c0->hw->clr_rd_req;

        // Increment the address

    }
    // Check to see if the I2C controller has finished sending data
    if (status & I2C_IC_INTR_STAT_R_TX_ABRT_BITS) {
        i2c0->hw->clr_tx_abrt;
        data_packets[packet_index].status.message_sent = true;
#ifdef I2C_DEBUG
        Serial.printf("TX abort detected\n");
#endif
        data_packets[packet_index].index = 0;
    }
    if (status & I2C_IC_INTR_STAT_R_STOP_DET_BITS) {
        i2c0->hw->clr_stop_det;
        data_packets[packet_index].status.message_sent = true;
#ifdef I2C_DEBUG
        Serial.printf("Stop detected\n");
#endif
        data_packets[packet_index].index = 0;
    }
    if (!data_packets[packet_index].status.message_sent)
        return;
    i2cReponse(packet_index);

}

void initPicoPeriph() {

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
    i2c0->hw->intr_mask = I2C_IC_INTR_MASK_M_RX_FULL_BITS | I2C_IC_INTR_MASK_M_RD_REQ_BITS |
                          I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS | I2C_IC_INTR_MASK_M_STOP_DET_BITS;

    // Set up the interrupt handler to service I2C interrupts
    irq_set_exclusive_handler(I2C0_IRQ, i2c0_irq_handler);

    // Enable I2C interrupt
    irq_set_enabled(I2C0_IRQ, true);
    Serial.printf("initialized the pico as a peripheral at address 0x%x\n", PICO_ADDRESS);
    int8_t motor_speeds = address_map[PICO_MOTOR_COMMAND_REGISTER];
    data_packets[motor_speeds].buffer[0] = 'O';
    data_packets[motor_speeds].buffer[1] = 'K';
}

void initPicoController() {
    gpio_init(SDA_PIN_0);
    gpio_set_function(SDA_PIN_0, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN_0);

    gpio_init(SCL_PIN_0);
    gpio_set_function(SCL_PIN_0, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN_0);

    i2c_init(i2c1, PICO_I2C_FREQ);
    Serial.printf("initialized the pico as a master\n");
}
