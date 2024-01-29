//
// Created by Øystein Bringsli.
//

#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_PID_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_PID_HPP


class PID {

public:
    double Kp;
    double Ki;
    double Kd;

    double prev_error = 0;
    double integral = 0;

    double setPoint = 0;

    double output(double input);

};


#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_PID_HPP
