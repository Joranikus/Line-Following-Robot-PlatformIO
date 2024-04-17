
#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP

#include <Arduino.h>
#include "MotorController.hpp"

class DirectionClass {
public:
    explicit DirectionClass(const int *pins, int antall_pins);
    double get_direction();
    bool is_left_turn_detected();
    bool is_right_turn_detected();
    void execute_90_degree_turn(MotorController &motor_controller, float speed_adjust, int turn_time, unsigned long &last_detection_time, unsigned long cooldown_time);

private:

    static int arr_sum(const int* inPins, int size);
    double this_direction();
    void read_sensor_pins();

    int antall_pins;
    int sensor_pins[7]{};
    int out_pins[7]{};
    double prev_direction = 0.0;
};

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
