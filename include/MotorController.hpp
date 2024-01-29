//
// Created by Øystein Bringsli.
//

#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP

class MotorController {
public:
    MotorController(double minValue, double maxValue);
    void motorControl(double analogValue, double speedAdjust);
    void PrintMotorSpeed(double currentDirection, double loopTime);

private:
    int motorPins[2] = {8, 9};
    int motor1PWM;
    int motor2PWM;

    double minValue;
    double maxValue;

    double steerValue = 0;

    int printInterval = 1000;
    int leftSpeed = 0;
    int rightSpeed = 0;
};


#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP
