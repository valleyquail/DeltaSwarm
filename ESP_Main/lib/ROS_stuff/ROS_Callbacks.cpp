//
// Created by nikesh on 3/24/24.
//


#include "../Robot/Robot.h"

#include <rcl/error_handling.h>
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


rcl_subscription_t movement_subscriber;

custom_messages__msg__RobotMovement robot_movement_msg;

void register_ros_callbacks(const void *robot) {

    rclc_executor_add_subscription(&executor, &movement_subscriber, &robot_movement_msg,
                                   &do_nothing, ON_NEW_DATA);

}

