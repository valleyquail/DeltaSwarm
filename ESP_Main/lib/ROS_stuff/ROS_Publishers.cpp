//
// Created by nikesh on 3/27/24.
//
#include "../../include/config.h"
#include "ROS_config.h"
#include "rcl/rcl.h"
#include "rcl/rclcpp"

inline void init_alive_msg() {
    std_msgs__msg__String__init(&alive_msg);
    alive_msg.data.data = (char *) malloc(ALIVE_MSG_CAPACITY);
    alive_msg.data.capacity = ALIVE_MSG_CAPACITY;
    snprintf(alive_msg.data.data, ALIVE_MSG_CAPACITY, "R%i Alive", ROBOT_NUM);
    alive_msg.data.size = strlen(alive_msg.data.data);
}

inline void init_odom_msg() {
    std_msgs__msg__String__init(&odom_msg);
    odom_msg.data.data = (char *) malloc(ODOM_MSG_CAPACITY);
    odom_msg.data.capacity = ODOM_MSG_CAPACITY;

}


void register_ros_publishers() {
    rcl_ret_t rc = rclc_publisher_init_default(&odom_pub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs),
                                               "odometry");
    rc += rclc_publisher_init_default(&robot_movement_pub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs),
                                      "robot_movement");

    if (rc != RCL_RET_OK) printf("Failed to create publisher\n");

}
