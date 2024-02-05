//
// Created by Øystein Bringsli.
//

#include <Arduino.h>
#include "MotorController.hpp"

double clamp(double val, double minValue, double maxValue) {
    return std::max(maxValue, std::min(minValue, val));
}

MotorController::MotorController(double minValue, double maxValue)
    : minValue(minValue), maxValue(maxValue) {

    pinMode(motor1PWM, OUTPUT);
    pinMode(motor2PWM, OUTPUT);
}

void MotorController::motorControl(double analogValue, double speedAdjust) {
    double analog = clamp(analogValue, minValue, maxValue);
    steerValue = static_cast<int>((analog - minValue) / (maxValue - minValue) * 255); //PWM uses 255

    leftSpeed = (steerValue > 128) ? 255 * speedAdjust : steerValue * 2 * speedAdjust;
    rightSpeed = (steerValue <128) ? 255 * speedAdjust : (steerValue - maxValue) * 2 * speedAdjust;

    // Set motor speeds using PWM
    sendSignal();
}

void MotorController::sendSignal() const {
    digitalWrite(motor1PWM, static_cast<int>(leftSpeed));
    digitalWrite(motor2PWM, static_cast<int>(rightSpeed));
}

void MotorController::PrintMotorSpeed(double currentDirection, double loopTime) const {
    static unsigned long lastPrintTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastPrintTime >= printInterval)
    {

        lastPrintTime = currentTime;

        Serial.print("Analog Value: ");
        Serial.print(currentDirection);
        Serial.print(" | Left Speed: ");
        Serial.print(leftSpeed);
        Serial.print(" | Right Speed: ");
        Serial.print(rightSpeed);
        Serial.print(" | Steer value: ");
        Serial.print(steerValue);
        Serial.print(" | Loop-time: ");
        Serial.print(loopTime);

        Serial.println();
    }
}