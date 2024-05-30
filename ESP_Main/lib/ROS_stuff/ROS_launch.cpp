//
// Created by nikesh on 5/28/24.
//
#include "../../include/config.h"
#include "ROS_config.h"
#include <rcl/error_handling.h>
#include <rcl/rcl.h>
#include <rcl/types.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>
#include <uros_network_interfaces.h>
#include "ROS_config.h"
#include <stdlib.h>


#define RCCHECK(fn)                                                                      \
    {                                                                                    \
        rcl_ret_t temp_rc = fn;                                                          \
        if ((temp_rc != RCL_RET_OK))                                                     \
        {                                                                                \
            printf("Failed status on line %d: %d. Aborting.\n", __LINE__, (int)temp_rc); \
            vTaskDelete(NULL);                                                           \
        }                                                                                \
    }
#define RCSOFTCHECK(fn)                                                                    \
    {                                                                                      \
        rcl_ret_t temp_rc = fn;                                                            \
        if ((temp_rc != RCL_RET_OK))                                                       \
        {                                                                                  \
            printf("Failed status on line %d: %d. Continuing.\n", __LINE__, (int)temp_rc); \
        }                                                                                  \
    }

void check_alive_timer_callback(rcl_timer_t *timer, int64_t last_call_time) {
    rcl_ret_t rc;
    UNUSED(last_call_time);
    if (timer != NULL) {
        rc = rcl_publish(&check_alive_pub, &alive_msg, NULL);
        if (rc == RCL_RET_OK) {
            printf("Published message %s\n", pub_msg.data.data);
        } else {
            printf("Error in timer_callback: Message %s could not be published\n", pub_msg.data.data);
        }
    } else {
        printf("Error in timer_callback: timer parameter is NULL\n");
    }
}

void do_nothing(const void *msgin) {
    // Do nothing

}

rcl_node_t node;
rcl_allocator_t allocator;
rclc_executor_t executor;
rclc_support_t support;


extern "C" void xLaunchROSNode(void *args) {


    allocator = rcl_get_default_allocator();
    executor = rclc_executor_get_zero_initialized_executor();
    rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
    // create init_options
    RCCHECK(rcl_init_options_init(&init_options, allocator));
    // Init support
    RCCHECK(rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator));

    //Init timer
    init_alive_msg();
    RCCHECK(rclc_publisher_init_default(&check_alive_pub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs),
                                        "check_alive"));
    RCCHECK(rclc_timer_init_default(&check_alive_timer, &support, timer_period, check_alive_timer_callback));

#ifdef CONFIG_MICRO_ROS_ESP_XRCE_DDS_MIDDLEWARE
    rmw_init_options_t* rmw_options = rcl_init_options_get_rmw_init_options(&init_options);

        // Static Agent IP and port can be used instead of autodisvery.
        RCCHECK(rmw_uros_options_set_udp_address(CONFIG_MICRO_ROS_AGENT_IP, CONFIG_MICRO_ROS_AGENT_PORT, rmw_options));
        // RCCHECK(rmw_uros_discover_agent(rmw_options));
#endif
    //Init Node
    RCCHECK(rclc_node_init_default(&node, name, "", &support));
    //Init Executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));

    unsigned int rcl_wait_timeout = 1000; // in ms

    RCCHECK(rclc_executor_set_timeout(&executor, RCL_MS_TO_NS(rcl_wait_timeout)));

    register_ros_callbacks();
    register_ros_publishers();

    while (1) {
        rclc_executor_spin(&executor);
    }

    // free resources
    RCCHECK(rcl_subscription_fini(&movement_subscriber, &node));
    RCCHECK(rcl_publisher_fini(&check_alive_pub, &node));
    RCCHECK(rcl_publisher_fini(&odom_pub, &node));
    RCCHECK(rcl_timer_fini(&check_alive_timer));
    std_msgs__msg__String__fini(&alive_msg);
    std_msgs__msg__String__fini(&odom_msg);
    std_msgs__msg__String__fini(&robot_movement_msg);
    RCCHECK(rcl_node_fini(&node));

    vTaskDelete(NULL);
}