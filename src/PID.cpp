//
// Created by Øystein Bringsli.
//

#include "PID.hpp"

PID::PID(double set_point, double Kp, double Kd, double Ki)
        : set_point(set_point), Kp(Kp), Kd(Kd), Ki(Ki) {}

double PID::output(double input) {
    auto error = set_point - input;

    auto prop = error * Kp;

    integral += error;
    integral *= Ki;

    auto derivative = (error - prev_error) * Kd;

    auto output = prop + integral + derivative;

    prev_error = error;

    // Limit output if necessary
    // if output > max_output:
    // output = max_output
    // else if output < min_output:
    // output = min_output

    return set_point - output;
}