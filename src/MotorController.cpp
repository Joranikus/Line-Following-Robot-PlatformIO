//
// Created by Øystein Bringsli.
//

#include <Arduino.h>
#include "MotorController.hpp"

double MotorController::clamp(double val, double minValue, double maxValue) {
    return std::max(minValue, std::min(maxValue, val));
}

MotorController::MotorController(double minValue, double maxValue)
    : minValue(minValue), maxValue(maxValue) {

    pinMode(motor1PWM, OUTPUT);
    pinMode(motor2PWM, OUTPUT);
}

void MotorController::motorControl(double analogValue, double speedAdjust) {
    double analog = clamp(analogValue, minValue, maxValue);
    steerValue = ((analog - minValue) / (maxValue - minValue)) * 255.0; //PWM uses 255

    leftSpeed = (steerValue > 128.0) ? 255.0 * speedAdjust : steerValue * 2.0 * speedAdjust;
    rightSpeed = (steerValue < 128.0) ? 255.0 * speedAdjust : (255.0 - steerValue) * 2.0 * speedAdjust;

    // Set motor speeds using PWM
    sendSignal();
}

void MotorController::sendSignal() const {
    ledcWrite(0, static_cast<int>(leftSpeed));
    ledcWrite(1, static_cast<int>(rightSpeed));
    //digitalWrite(motor1PWM, static_cast<int>(leftSpeed));
    //digitalWrite(motor2PWM, static_cast<int>(rightSpeed));
}

void MotorController::PrintMotorSpeed(double currentDirection, double loopTime, double dirWithoutPid) const {
    static unsigned long lastPrintTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastPrintTime >= printInterval)
    {
        lastPrintTime = currentTime;

        Serial.print("Analog Value: ");
        Serial.print(currentDirection);
        Serial.print(" | Without pid: ");
        Serial.print(dirWithoutPid);
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