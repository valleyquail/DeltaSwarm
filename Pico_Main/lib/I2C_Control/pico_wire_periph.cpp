//
// Created by nikesh on 9/18/24.
//
#include <Arduino.h>
#include <Wire.h>
#include "pico_wire_periph.h"
#include "../../include/config.h"
#include "i2c_control.h"

// Easy way to store both the index of the data packet and the length of the data stored in the packet
struct DataParameters {
    int8_t _array_index;
    int8_t _data_len;
};
//Hashmap of function pointers to call when a packet is received
std::unordered_map<int, i2c_response_t> i2cFunctions;
/*
 * This is a hashmap that maps the packet address to the index of the data packet so that the addresses in the Pico
 * registers do not have to be consecutive
 */

std::unordered_map<uint8_t, struct DataParameters> address_map;
int8_t data_packet_access_assignment_index = 0;

uint8_t pico_register;

// Function to register a function to be called when a packet is received by adding it to a hashmap
void register_i2c_function(i2c_response_t function, int8_t packet_address, int8_t data_len) {
    i2cFunctions[packet_address] = function;
    address_map[packet_address] = DataParameters{data_packet_access_assignment_index++, data_len};
//    Serial.printf("Registered function at address %d to index %d\n", packet_address, address_map[packet_address]);
}

// Function to call the function registered to a packet address
inline void i2cResponse(int curr_packet_address) {
    if (i2cFunctions.find(curr_packet_address) != i2cFunctions.end()) {
        i2cFunctions[curr_packet_address](address_map[curr_packet_address]._array_index);
    }
}


struct DataPacket data_packets[NUM_PICO_REGISTERS];
DataPacket *current_packet;
static char pico_buffer[33];

void onReceive(int numBytes) {
    for (int i = 0; i < numBytes; i++) {
        pico_buffer[i] = Wire1.read();
    }
#ifdef I2C_DEBUG
    for (int i = 0; i < numBytes; i++) {
        Serial.printf("%c", pico_buffer[i]);
    }
    Serial.printf("\n");
#endif
    pico_register = pico_buffer[0];
    current_packet = &data_packets[address_map[pico_register]._array_index];
    current_packet->data_len = address_map[pico_register]._data_len;
    // If the packet is only the register byte, then there is no data to write, just return
    if (numBytes == 1)
        return;
    memcpy(current_packet->buffer, pico_buffer + 1, numBytes - 1);
    current_packet->status.message_sent = false;
    i2cResponse(pico_register);
}

void onRequest() {
    Wire1.write(current_packet->buffer, current_packet->data_len);
#ifdef DEBUG
    Serial.printf("Sent:");
    for (int i = 0; i < current_packet->data_len; i++) {
        Serial.printf("%c", current_packet->buffer[i]);
    }
    Serial.printf("\n");
#endif
    current_packet->status.message_sent = true;
}


void initPicoPeriph() {
    // Set up the device as a peripheral
    Wire1.setSDA(SDA_PIN_1);
    Wire1.setSCL(SCL_PIN_1);
    Wire1.setClock(PICO_ESP_FREQ);
    Wire1.begin(PICO_ADDRESS);
    Wire1.onRequest(onRequest);
    Wire1.onReceive(onReceive);
    Serial.printf("initialized the pico as a peripheral at address 0x%x\n", PICO_ADDRESS);
    current_packet = &data_packets[address_map[TEST_CONNECTION_REGISTER]._array_index];
    current_packet->buffer[1] = 'x';
    current_packet->buffer[2] = 'x';
}
