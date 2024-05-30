//
// Created by nikesh on 5/28/24.
//

#ifndef PICO_MAIN_I2C_SENSOR_H
#define PICO_MAIN_I2C_SENSOR_H

class I2C_Sensor {

private:

public:
    const float noise = 0;
    const float bias = 0;
    const float scale = 0;
    float update_freq;

    I2C_Sensor(float update_freq);

    bool init();

    //In case the frequency needs to be dynamically changed
    void update_update_freq(float new_update_freq);
};


#endif //PICO_MAIN_I2C_SENSOR_H
