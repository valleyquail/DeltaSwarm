

// #include "i2c_control.h"
// #include "../../include/config.h"
// #include "../../include/pin_definitions.h"
// #include "hardware/i2c.h"
// // bruh

// void espRequestHandler(i2c_inst_t *i2c, i2c_slave_event_t event);

// // Copy the motion controller in so that it can send over encoder counts
// // extern MotionController motionController;

// enum I2C_COMMANDS
// {
//     MOTOR_SPEEDS = 0,
//     ENCODER_VALUES = 1,
//     IMU_VALUES = 2,
//     NONE = 255,
// };
// uint8_t current_command;

// DataPacket motor_speeds;
// DataPacket encoder_values;
// DataPacket imu_values;

// DataPacket *current_packet;

// void initPicoPeriph()
// {
//     gpio_init(SDA_PIN_1);
//     gpio_set_function(SDA_PIN_1, GPIO_FUNC_I2C);
//     gpio_pull_up(SDA_PIN_1);

//     gpio_init(SCL_PIN_1);
//     gpio_set_function(SCL_PIN_1, GPIO_FUNC_I2C);
//     gpio_pull_up(SCL_PIN_1);

//     i2c_init(i2c0, PICO_ESP_FREQ);
//     // configure I2C0 for slave mode
//     i2c_slave_init(i2c0, PICO_ADDRESS, &espRequestHandler);
//     // printf("initialized the pico as a client at address %i\n", PICO_ADDRESS);
// }

// void initPicoMaster()
// {
//     gpio_init(SDA_PIN_0);
//     gpio_set_function(SDA_PIN_0, GPIO_FUNC_I2C);
//     gpio_pull_up(SDA_PIN_0);

//     gpio_init(SCL_PIN_0);
//     gpio_set_function(SCL_PIN_0, GPIO_FUNC_I2C);
//     gpio_pull_up(SCL_PIN_0);

//     i2c_init(i2c1, PICO_I2C_FREQ);
//     // printf("initialized the pico as a master\n");
// }

// void espRequestHandler(i2c_inst_t *i2c, i2c_slave_event_t event)
// {
//     switch (event)
//     {
//     case I2C_SLAVE_RECEIVE: // master has written some data
//     {                       // read the data
//         // If this is the fist byte, then it is the command
//         if (current_command == NONE)
//         {
//             current_command = i2c_read_byte_raw(i2c0);
//             // Used to determine which packet to send based on the command sent
//             // over I2C
//             switch (current_command)
//             {
//             case MOTOR_SPEEDS:
//                 current_packet = &motor_speeds;
//                 break;

//             case ENCODER_VALUES:
//                 current_packet = &encoder_values;
//                 break;
//             case IMU_VALUES:
//                 current_packet = &imu_values;
//                 break;
//             default:
//                 break;
//             }
//             current_packet->index = 0;
//             current_packet->status.is_receiving_message = true;
//         }
//         else
//         {
//             current_packet->buffer[current_packet->index++] = i2c_read_byte_raw(i2c0);
//         }
//         break;
//     }
//     case I2C_SLAVE_REQUEST: // master is requesting data
//         i2c_write_byte_raw(i2c0, current_packet->buffer[current_packet->index++]);
//         break;
//     case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
//         current_packet->status.is_receiving_message = false;
//         current_packet->index = 0;
//         current_packet = NULL;
//         if (current_command == MOTOR_SPEEDS)
//         {
//             // Set the motor speeds
//             // motionController.setSpeed(current_packet->buffer[0],
//             // current_packet->buffer[1], current_packet->buffer[2]);
//             // printf("buffer: %s\n", current_packet->buffer);
//         }
//         current_command = NONE;
//         break;
//     default:
//         break;
//     }
// }
