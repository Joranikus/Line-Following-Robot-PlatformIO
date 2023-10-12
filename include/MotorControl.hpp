//
// Created by Ørjan on 09/10/2023.
//

#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROL_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROL_HPP

#include <Arduino.h>
namespace mot {

    class MotorControl {

    public:

        MotorControl(const int pins[], int antallPins) : antallPins(antallPins) {

            for (int i = 0; i < antallPins; i++)
            {
                pinMode(pins[i], OUTPUT);
            }

        }

        void motorPinSetup() {
            digitalWrite(motorSTBY, HIGH);

            digitalWrite(leftMotorAIN1, HIGH);
            digitalWrite(leftMotorAIN2, LOW);

            digitalWrite(rightMotorBIN1, HIGH);
            digitalWrite(rightMotorBIN2, LOW);
        }

        void printMotorSpeed(unsigned long interval, double dirInput)
        {
            static unsigned long lastPrintTime = 0;
            unsigned long currentTime = millis();

            if (currentTime - lastPrintTime >= interval)
            {
                //For loop timer
                //unsigned long dt = currentTime - lastPrintTime;

                lastPrintTime = currentTime;

                Serial.print("Analog Value: ");
                Serial.print(dirInput);
                Serial.print(" | Left Speed: ");
                Serial.print(leftSpeed);
                Serial.print(" | Right Speed: ");
                Serial.print(rightSpeed);
                Serial.print(" | Steer value: ");
                Serial.print(steerValue);
                Serial.println();

            }
        }

        // Function to control motors based on analog input and its range
        void motorControl(double analogValue, int minValue, int maxValue, float speedAdjust) {

            int analog = max(minValue, min(analogValue, maxValue));

            steerValue = (analog - minValue) / (maxValue - minValue) * 255;

            // Apply the speed adjustment
            if (steerValue > 128) {
                // Steer right
                leftSpeed = 255;
                rightSpeed = (255 - steerValue) * 2;
            } else if (steerValue < 128) {
                // Steer left
                leftSpeed = steerValue * 2;
                rightSpeed = 255;
            } else {
                // No steering, both motors forward
                leftSpeed = 255;
                rightSpeed = 255;
            }

            leftSpeed *= speedAdjust;
            rightSpeed *= speedAdjust;

            analogWrite(leftMotorPWM, leftSpeed);
            analogWrite(rightMotorPWM, rightSpeed);
        }

    private:

        int leftSpeed;
        int rightSpeed;
        int motorSTBY;
        int steerValue;

        int leftMotorPWM;
        int leftMotorAIN1;
        int leftMotorAIN2;

        int rightMotorPWM;
        int rightMotorBIN1;
        int rightMotorBIN2;

        int antallPins;
        int outputPins[];

    };
}

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROL_HPP