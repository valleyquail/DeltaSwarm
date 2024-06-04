# ROS Configuration

This is a collection of ROS configuration files and scripts that I use to setup my ROS workspace. The files are
organized in the following way:

- `ROS_config.h`: This file contains the configuration of the ROS workspace. It houses the ROS package names, the ROS
  node names, the ROS topic names, the ROS message types, and the ROS service types.
- `ROS_Callbacks.h`: This file contains the callback functions for the ROS nodes. It houses the callback functions for
  the ROS publishers, the ROS subscribers, the ROS services, and the ROS actions.
- `ROS_Publishers.h`: This file contains the publisher functions for the ROS nodes. It houses the publisher functions
  for the ROS publishers.
- `ROS_Subscribers.h`: This file contains the subscriber functions for the ROS nodes. It houses the subscriber functions
- `ROS_launch.h`: This file contains the main thread for launching and configuring the ROS nodes. It houses the main
  thread for launching and configuring the ROS nodes.

# Note

You need to go into the microros component and change the `microros_agent` to the correct value for your system.
Currently, the max number of subscriptions and publishers are set to 8

## TODO:

- [ ] Add more ROS configuration files and scripts.
- [ ] Make a nice API so that everything is better handled and organized without the user having to go into the
  files directly