
#include <Arduino.h>

#include "MotorController.hpp"
#include "PID.hpp"

int antallPins = 7;
int sensorPins[7] = {22, 23, 16, 17, 5, 18, 21};
float prev_dir;

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

    bool sensor_activations[7];
    int sum = 0;
    for (int i = 0; i < antallPins; i++)
    {
        sensor_activations[i] = digitalRead(sensorPins[i]);
        sum += sensor_activations[i];

    }

    float dir;
    if ((sum == 0) || (sum == antallPins)) {
        dir = prev_dir;
    } else {
        dir = direction(sensor_activations, antallPins);
        prev_dir = dir;
    }

    motorController.motorControl(dir, 0.7);

    auto endTime = millis();

    motorController.PrintMotorSpeed(dir, endTime - startTime);
}