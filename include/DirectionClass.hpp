
#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP

#include <Arduino.h>

class DirectionClass {
public:
    explicit DirectionClass(const int *pins, int antall_pins);
    double get_direction();

private:

    static int arr_sum(const int* inPins, int size);
    double this_direction();

    int antall_pins;
    int sensor_pins[7]{};
    int out_pins[7]{};
    void read_sensor_pins();
    double prev_direction = 0.0;
};

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
