#include <Arduino.h>
#include "DirectionClass.hpp"
#include "MotorControl.hpp"

////////////////////////////////////Adjustables////////////////////////////////////////////

int printIntervalMilliseconds = 250;

float speedAdjust = 0.6;
int sensorLimit = 250;

double Kp = 0.8;
double Ki = 0;
double Kd = 0;


///////////////////////////////////////////////////////////////////////////////////////////

int sensorPins[5] = {
        A0, // Sensor 1
        A1, // Sensor 2
        A2, // Sensor 3
        A3, // Sensor 4
        A4, // Sensor 5
};

int outputPins[7] = {
        9,  // motorSTBY
        5,  // leftMotorPWM
        2,  // leftMotorAIN1
        3,  // leftMotorAIN2
        6,  // rightMotorPWM
        7,  // rightMotorBIN1
        8,  // rightMotorBIN2
};

dir::DirectionClass directionClass{sensorPins, 5, Kp, Ki, Kd, sensorLimit};
mot::MotorControl motorController{outputPins, 7};

void setup()
{
    Serial.begin(9600);
    motorController.motorPinSetup();
}

void loop()
{
    motorController.motorControl(directionClass.get_direction(), -1, 1, speedAdjust);
    motorController.printMotorSpeed(printIntervalMilliseconds, directionClass.get_direction());
}