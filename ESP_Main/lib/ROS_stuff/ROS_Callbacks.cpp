//
// Created by nikesh on 3/24/24.
//


#include "../Robot/Robot.h"


#include <rcl/rcl.h>
#include <rcl/types.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>
#include <uros_network_interfaces.h>
#include "ROS_config.h"

#include "custom_messages/msg/robot_movement.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../Robot/Robot.h"


void movement_sub_callback(const void *movement) {
    const custom_messages__msg__RobotMovement *move = (custom_messages__msg__RobotMovement *) movement;
    robot.setRobotMotion(move->speed, move->theta, move->omega, move->keep_orientation);
#ifdef DEBUG
    printf("Speed: %f, Theta: %f, Omega: %f, Orientation %c\n", move->speed, move->theta, move->omega,
           move->keep_orientation);
#endif
}

void register_ros_callbacks(const void *args) {

//    rcl_ret_t rc = 0;
    rclc_subscription_init_default(&movement_sub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT
    (custom_messages, msg, RobotMovement), "movement");

    rclc_executor_add_subscription(&executor, &movement_sub, &robot_movement_msg,
                                   &movement_sub_callback, ON_NEW_DATA);

}



