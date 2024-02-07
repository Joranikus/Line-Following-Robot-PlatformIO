//
// Created by Øystein Bringsli.
//

#include "PID.hpp"

double PID::output(double input) {
    auto error = setPoint - input;

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

    return setPoint - output;
}

PID::PID(double setPoint, double Kp, double Kd, double Ki)
    : setPoint(setPoint), Kp(Kp), Kd(Kd), Ki(Ki) {}
