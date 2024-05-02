//
// Created by Øystein Bringsli.
//

#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_PID_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_PID_HPP


class PID {

public:
    PID(double set_point = 0, double Kp = 0, double Kd = 0, double Ki = 0);

    double Kp;
    double Ki;
    double Kd;

    double error = 0;
    double prev_error = 0;
    double integral = 0;
    double derivative = 0;
    double prev_derivative = 0;

    double set_point;

    double output(double input, double dt);

};


#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_PID_HPP
