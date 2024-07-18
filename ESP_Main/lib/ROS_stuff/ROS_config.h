// Stack of the micro-ROS app in bytes
#define MICRO_ROS_APP_STACK 16000

// Priority of the micro-ROS app
#define MICRO_ROS_APP_TASK_PRIO 5

#include "rcl/rcl.h"
#include "rclc/rclc.h"
#include <rclc/executor.h>
#include <stdlib.h>
#include "std_msgs/msg/string.h"
#include "custom_messages/msg/robot_movement.h"
#include <nav_msgs/msg/odometry.h>
#include <string>


//Number of handles the executor will use: sum of the number of publishers, subscribers and timers
const int num_handles = 3;

//Node configuration
//______________________________________________________________________________________________________________________

extern rcl_node_t node;
extern rcl_allocator_t allocator;
extern rclc_executor_t executor;
extern rclc_support_t support;

//Timers
//______________________________________________________________________________________________________________________
extern rcl_timer_t check_alive_timer;
//publish alive once a second
const uint32_t timer_period = RCL_MS_TO_NS(1000);
void check_alive_timer_callback(rcl_timer_t * timer, int64_t last_call_time);

//Timer Message
//______________________________________________________________________________________________________________________
extern std_msgs__msg__String alive_msg;
const uint8_t ALIVE_MSG_CAPACITY = 16;


//Subscribers
//______________________________________________________________________________________________________________________
extern rcl_subscription_t movement_sub;

//Subscriber Messages
//______________________________________________________________________________________________________________________
extern custom_messages__msg__RobotMovement robot_movement_msg;
const uint8_t ROBOT_MOVEMENT_MSG_CAPACITY = 16;


//Publishers
//______________________________________________________________________________________________________________________
extern rcl_publisher_t check_alive_pub;
extern rcl_publisher_t odom_pub;
extern rcl_publisher_t robot_movement_pub;

//Publisher Messages
//______________________________________________________________________________________________________________________
extern nav_msgs__msg__Odometry odom_msg;


//Launch ROS node
extern "C" void xLaunchROSNode(const void *args);

//Register ROS publishers and subscribers
extern "C" void register_ros_publishers(const void *args);

extern "C" void register_ros_callbacks(const void *args);

//Initialize required messages
inline void init_all_msg();