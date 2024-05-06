
#include <Arduino.h>
#include "PID.hpp"

PID::PID(double set_point, double Kp, double Kd, double Ki)
        : set_point(set_point), Kp(Kp), Kd(Kd), Ki(Ki) {}

double PID::output(double input, double dt) {
    error = set_point - input;

    if (abs(error) < 1) {integral = 0;}
    integral *= 0.999;
    integral += (error + prev_error) * dt / 2000000.0;

    if (integral < -100.0) {
        integral = -100.0;
    } else if (integral > 100.0) {
        integral = 100.0;
    }

    constexpr double u = 0.001;
    constexpr double v = 1 - u;
    derivative = (error - prev_error) * dt * u + prev_derivative * v;
    prev_derivative = derivative;

    auto output = (error * Kp) + (integral * Ki) + (derivative * Kd);

    prev_error = error;

    return set_point - output;
}