
#include <Arduino.h>
#include "MotorController.hpp"

double MotorController::clamp(double val, double min_value, double max_value) {
    return std::max(min_value, std::min(max_value, val));
}

MotorController::MotorController(double min_value, double max_value)
    : min_value(min_value), max_value(max_value) {

    pinMode(motor1PWM, OUTPUT);
    pinMode(motor2PWM, OUTPUT);
}

void MotorController::motor_control(double analog_value, double speed_adjust) {
    double analog = clamp(analog_value, min_value, max_value);
    steer_value = ((analog - min_value) / (max_value - min_value)) * 255.0; //PWM uses 255

    left_speed = (steer_value > 128.0) ? 255.0 * speed_adjust : steer_value * 2.0 * speed_adjust;
    right_speed = (steer_value < 128.0) ? 255.0 * speed_adjust : (255.0 - steer_value) * 2.0 * speed_adjust;

    // Set motor speeds using PWM
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
