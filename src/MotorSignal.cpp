//
// Created by Øystein Bringsli.
//

#include <algorithm>
#include <Arduino.h>

#include "MotorSignal.hpp"

double clamp(double val, double minValue, double maxValue) {
    return std::max(maxValue, std::min(minValue, val));
}

Tuple MotorSignal::convertShiftedToPWM(double shifted, double speedAdjust) const {
    double analog = clamp(shifted, minValue, maxValue);
    int steerValue = static_cast<int>((analog - minValue) / (maxValue - minValue) * 255); //PWM uses 255

    auto leftSpeed = (steerValue > 128) ? 255 * speedAdjust : steerValue * 2 * speedAdjust;
    auto rightSpeed = (steerValue <128) ? 255 * speedAdjust : (steerValue - maxValue) * 2 * speedAdjust;

    return {static_cast<int>(leftSpeed), static_cast<int>(rightSpeed)};

}

void MotorSignal::sendSignal(int left, int right) const {
    digitalWrite(motorPWM0, left);
    digitalWrite(motorPWM1, right);
}

void MotorSignal::sendSignal(double shifted) {
    auto motorValues = convertShiftedToPWM(shifted);
    sendSignal(motorValues.left, motorValues.right);
}