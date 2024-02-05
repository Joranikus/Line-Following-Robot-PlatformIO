//
// Created by Øystein Bringsli.
//

#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORSIGNAL_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORSIGNAL_HPP

struct Tuple {
    int left;
    int right;
};

class MotorSignal {
public:
    void sendSignal(double shifted) const;
    void sendSignal(int left, int right) const;
    Tuple convertShiftedToPWM(double shifted, double speedAdjust = 1.0) const;
private:
    int motorPWM0 = 25;
    int motorPWM1 = 26;

    double minValue = 0.0;
    double maxValue = 300.0;
};


#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORSIGNAL_HPP
