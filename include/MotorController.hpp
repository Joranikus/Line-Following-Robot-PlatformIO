//
// Created by Øystein Bringsli.
//

#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP

class MotorController {
public:
    MotorController(double minValue, double maxValue);
    void motorControl(double analogValue, double speedAdjust);
    void PrintMotorSpeed(double currentDirection, double loopTime) const;

private:
    int motor1PWM = 25;
    int motor2PWM = 26;

    double minValue = 0;
    double maxValue = 300;

    double steerValue = 0;

    int printInterval = 1000;
    double leftSpeed = 0;
    double rightSpeed = 0;
};


#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP
