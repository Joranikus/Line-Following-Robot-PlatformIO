
#include <Arduino.h>

#include "DirectionClass.hpp"
#include "MotorController.hpp"
#include "PID.hpp"
#include "Tests.hpp"

int numSensorPins = 7;
int sensorPins[7] = {22, 23, 16, 17, 5, 18, 21};

DirectionClass directionClass{sensorPins, numSensorPins};
MotorController motorController{0, 300};
PID pid;

Tests tests;

void setup()
{

    ledcSetup(0, 300000, 8);
    ledcSetup(1, 300000, 8);

    ledcAttachPin(25, 0);
    ledcAttachPin(26, 1);

    Serial.begin(9600);
    Serial.println();
    Serial.println("Setup complete.");
}

unsigned long lastPrintTime = 0;
void loop()
{
    auto startTime = millis();

    auto dir = directionClass.get_direction();
    motorController.motorControl(dir, 1.0);

    auto endTime = millis();
    motorController.PrintMotorSpeed(dir, endTime - startTime);

    auto currentTime = millis();
    if (currentTime > (lastPrintTime + 500)) {
        tests.printSensors(sensorPins, numSensorPins);
        lastPrintTime = currentTime;
    }
}