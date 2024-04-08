//
// Created by nikesh on 3/27/24.
//

#ifndef ESP_MAIN_ROBOT_H
#define ESP_MAIN_ROBOT_H

struct RobotMotion
{
    float x;
    float y;
    float z;
    float roll;
    float pitch;
    float yaw;
};

class Robot
{
private:
    struct RobotMotion;
    int robot_number;

public:
    Robot(int robot_number);
    bool init();


    void setRobotMotion();
    // Getters
    const struct RobotMotion getRobotMotion();
    int getRobotNumber();
};

#endif // ESP_MAIN_ROBOT_H
