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
    // integral *= Ki;
    if (integral < -100.0) {
        integral = -100.0;
    } else if (integral > 100.0) {
        integral = 100.0;
    }

    auto derivative = (error - prev_error) * Kd * 3000;

    auto output = prop + integral * Ki + derivative;

    prev_error = error;

    return set_point - output;
}