
#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP

#include <Arduino.h>
#include <array>
#include "MotorController.hpp"

enum ExtremeTurnDirection {
    LEFT,
    RIGHT,
    OFF
};

class DirectionClass {
public:
    explicit DirectionClass(const int *pins, int antall_pins);
    double get_direction();
    void updateExtremeTurn();
    void execute_90_degree_turn(MotorController &motor_controller, float speed_adjust, int turn_time, unsigned long &last_detection_time, unsigned long cooldown_time);
    ExtremeTurnDirection extremeTurnDirection = OFF;
    bool extremeTurnActive = false;

private:

    bool is_left_turn_detected();
    bool is_right_turn_detected();

    static int arr_sum(const int* inPins, int size);
    double this_direction();
    void read_sensor_pins();

    int numPins;
    int sensor_pins[7]{};
    std::array<int, 7> prevSensor = {0, 0, 0, 0, 0, 0, 0};
    int out_pins[7]{};
    double prev_direction = 0.0;
    int nullSensorIters = 0;
};

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
