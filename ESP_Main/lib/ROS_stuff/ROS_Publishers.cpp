//
// Created by nikesh on 3/27/24.
//
#include "../../include/config.h"
#include "ROS_config.h"

nav_msgs__msg__Odometry odom_msg;
rcl_publisher_t check_alive_pub;
rcl_publisher_t odom_pub;
rcl_publisher_t robot_movement_pub;

void register_ros_publishers(const void *args) {
    rcl_ret_t rc = rclc_publisher_init_default(&odom_pub, &node,
                                               ROSIDL_GET_MSG_TYPE_SUPPORT(nav_msgs, msg, Odometry),
                                               "odom");

    rc += rclc_publisher_init_default(&robot_movement_pub, &node,
                                      ROSIDL_GET_MSG_TYPE_SUPPORT(custom_messages, msg, RobotMovement),
                                      "robot_movement");


    if (rc != RCL_RET_OK) printf("Failed to create publisher\n");

}
