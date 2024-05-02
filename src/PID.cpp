//
// Created by Øystein Bringsli.
//

#include <Arduino.h>
#include "PID.hpp"

PID::PID(double set_point, double Kp, double Kd, double Ki)
        : set_point(set_point), Kp(Kp), Kd(Kd), Ki(Ki) {}

/*double PID::output(double input) {
    auto error = set_point - input;



    auto prop = error * Kp;

    integral += error;
    // integral *= Ki;
    if (integral < -100.0) {
        integral = -100.0;
    } else if (integral > 100.0) {
        integral = 100.0;
    }

    auto derivative = (error - prev_error) * Kd;

    auto output = prop + integral * Ki + derivative;

    prev_error = error;

    return set_point - output;
}*/

/*double PID::output(double input, double dt) {
    error = set_point - input;

    auto prop = error * Kp;

    integral += error * dt; // Accumulate error with respect to time

    // Apply saturation limits to the integral term
    if (integral < -100.0) {
        integral = -100.0;
    } else if (integral > 100.0) {
        integral = 100.0;
    }

    derivative = (error - prev_error) / dt; // Compute derivative

    auto output = prop + integral * Ki + derivative * Kd;

    prev_error = error;

    return set_point - output;
}*/

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