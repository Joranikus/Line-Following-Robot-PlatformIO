//
// Created by Øystein Bringsli.
//

#include <Arduino.h>
#include "MotorController.hpp"

MotorController::MotorController(double minValue, double maxValue)
    : minValue(minValue), maxValue(maxValue) {
    motor1PWM = motorPins[0];
    motor2PWM = motorPins[1];

    pinMode(motor1PWM, OUTPUT);
    pinMode(motor2PWM, OUTPUT);
}

void MotorController::motorControl(double analogValue, double speedAdjust) {
    double analog = max(minValue, min(analogValue, maxValue));
    int steer_value = (analog - minValue) / (maxValue - minValue) * 255;
    steerValue = steer_value;
    //double steer_val_double = analogValue * 255;
    //int steer_value = static_cast<int>(steer_val_double);

    //Serial.println(steer_value);

    // Apply the speed adjustment
    if (steer_value > 128) {
        // Steer right
        leftSpeed = 255;
        rightSpeed = (255 - steer_value) * 2;
    } else if (steer_value < 128) {
        // Steer left
        leftSpeed = steer_value * 2;
        rightSpeed = 255;
    } else {
        // No steering, both motors forward
        leftSpeed = 255;
        rightSpeed = 255;
    }

    leftSpeed = leftSpeed * speedAdjust;
    rightSpeed = rightSpeed * speedAdjust;

    // Set motor speeds using PWM
    digitalWrite(motor1PWM, leftSpeed);
    digitalWrite(motor2PWM, rightSpeed);
}

void MotorController::PrintMotorSpeed(double currentDirection, double loopTime, HardwareSerial &serial) {
    static unsigned long lastPrintTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastPrintTime >= printInterval)
    {

        lastPrintTime = currentTime;

        serial.print("Analog Value: ");
        serial.print(currentDirection);
        serial.print(" | Left Speed: ");
        serial.print(leftSpeed);
        serial.print(" | Right Speed: ");
        serial.print(rightSpeed);
        serial.print(" | Steer value: ");
        serial.print(steerValue);
        serial.print(" | Looptime: ");
        serial.print(loopTime);

        serial.println();
    }
}