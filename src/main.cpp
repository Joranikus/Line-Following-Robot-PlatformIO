
#include <Arduino.h>

#include "DirectionClass.hpp"
#include "MotorController.hpp"
#include "PID.hpp"

int antallPins = 7;
int sensorPins[7] = {22, 23, 16, 17, 5, 18, 21};


DirectionClass directionClass{sensorPins, antallPins};
PID pid;
MotorController motorController{0, 300};

void setup()
{
    Serial.begin(9600);
    Serial.println();
    Serial.println("Setup complete.");
}

void loop()
{
    auto startTime = millis();

    auto dir = directionClass.get_direction();
    motorController.motorControl(dir, 0.7);

    auto endTime = millis();

    motorController.PrintMotorSpeed(dir, endTime - startTime);
}