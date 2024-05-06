
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
    explicit DirectionClass(std::array<short, 7> pins);
    double get_direction();
    void updateExtremeTurn();
    void execute_90_degree_turn(MotorController &motor_controller, float speed_adjust, int turn_time, unsigned long &last_detection_time, unsigned long cooldown_time);
    ExtremeTurnDirection extremeTurnDirection = OFF;
    bool extremeTurnActive = false;

private:

    bool is_left_turn_detected();
    bool is_right_turn_detected();

    static int arraySum(std::array<short, 7> &pins);
    double this_direction();
    void read_sensor_pins();

    int numPins;
    std::array<short, 7> sensor_pins;
    std::array<short, 7> out_pins;
    std::array<int, 7> prevSensor = {0, 0, 0, 0, 0, 0, 0};
    std::array<int, 7> prevActiveSensor = {0, 0, 0, 0, 0, 0, 0};

    double prev_direction = 0.0;
    int nullSensorIters = 0;

    bool waitForDetection(int maxTime);
};

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
