
#ifndef LINEFOLLOWINGROBOT_TESTS_HPP
#define LINEFOLLOWINGROBOT_TESTS_HPP

#include "MotorController.hpp"

class Tests {
public:


    void print_sensors(int *pins, int num_pins, int print_delay) const;

    void print_motor_speed(MotorController& motor_controller, double dir_without_pid,
                           double current_direction, int print_delay) const;
};


#endif //LINEFOLLOWINGROBOT_TESTS_HPP
