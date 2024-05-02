
#include <Arduino.h>
#include "MotorController.hpp"

double MotorController::clamp(double val, double min_value, double max_value) {
    return std::max(min_value, std::min(max_value, val));
}

MotorController::MotorController(double min_value, double max_value)
    : min_value(min_value), max_value(max_value) {

    pinMode(motor1PWM, OUTPUT);
    pinMode(motor2PWM, OUTPUT);

    pinMode(motor_1_forward, OUTPUT);
    pinMode(motor_1_reverse, OUTPUT);
    pinMode(motor_2_forward, OUTPUT);
    pinMode(motor_2_reverse, OUTPUT);
}

void MotorController::motor_control_backward(double analog_value, double speed_adjust) {
    double analog = clamp(analog_value, min_value, max_value);
    steer_value = ((analog - min_value) / (max_value - min_value)) * 255.0; //PWM uses 255

    left_speed = (steer_value > 128.0) ? 255.0 * speed_adjust : steer_value * 2.0 * speed_adjust;
    right_speed = (steer_value < 128.0) ? 255.0 * speed_adjust : (255.0 - steer_value) * 2.0 * speed_adjust;

    digitalWrite(motor_1_forward, 0);
    digitalWrite(motor_1_reverse, 1);
    digitalWrite(motor_2_forward, 0);
    digitalWrite(motor_2_reverse, 1);

    send_signal();
}

void MotorController::motor_control_forward(double analog_value, double speed_adjust) {
    double analog = clamp(analog_value, min_value, max_value);
    steer_value = ((analog - min_value) / (max_value - min_value)) * 255.0; //PWM uses 255

    left_speed = (steer_value > 128.0) ? 255.0 * speed_adjust : steer_value * 2.0 * speed_adjust;
    right_speed = (steer_value < 128.0) ? 255.0 * speed_adjust : (255.0 - steer_value) * 2.0 * speed_adjust;

    digitalWrite(motor_1_forward, 1);
    digitalWrite(motor_1_reverse, 0);
    digitalWrite(motor_2_forward, 1);
    digitalWrite(motor_2_reverse, 0);

    send_signal();
}
/*
void MotorController::motor_control_forward(double analog_value, double speed_adjust) {
    double analog = clamp(analog_value, min_value, max_value);
    steer_value = ((analog - min_value) / (max_value - min_value)) * 255.0; //PWM uses 255

    left_speed = 255.0 * speed_adjust; // Set default speed for both motors
    right_speed = 255.0 * speed_adjust;

    // Adjust speed for turning
    if (steer_value < 128.0) { // If turning left
        left_speed *= (2.0 * speed_adjust); // Increase left motor speed for turning
    } else if (steer_value > 128.0) { // If turning right
        right_speed *= (2.0 * speed_adjust); // Increase right motor speed for turning
    }

    digitalWrite(motor_1_forward, 1);
    digitalWrite(motor_1_reverse, 0);
    digitalWrite(motor_2_forward, 1);
    digitalWrite(motor_2_reverse, 0);

    send_signal();
}*/

void MotorController::motor_control_left_turn(double speed_adjust) {
    left_speed = 255 * speed_adjust;
    right_speed = 255 * speed_adjust;

    digitalWrite(motor_1_forward, 0);
    digitalWrite(motor_1_reverse, 1);
    digitalWrite(motor_2_forward, 1);
    digitalWrite(motor_2_reverse, 0);

    send_signal();
}

void MotorController::motor_control_right_turn(double speed_adjust) {
    left_speed = 255 * speed_adjust;
    right_speed = 255 * speed_adjust;

    digitalWrite(motor_1_forward, 1);
    digitalWrite(motor_1_reverse, 0);
    digitalWrite(motor_2_forward, 0);
    digitalWrite(motor_2_reverse, 1);

    send_signal();
}

void MotorController::send_signal() const {
    ledcWrite(0, static_cast<int>(left_speed));
    ledcWrite(1, static_cast<int>(right_speed));
    //digitalWrite(motor1PWM, static_cast<int>(left_speed));
    //digitalWrite(motor2PWM, static_cast<int>(right_speed));
}

double MotorController::get_left_speed() {
    return left_speed;
}

double MotorController::get_right_speed() {
    return right_speed;
}

double MotorController::get_steer_value() {
    return steer_value;
}
