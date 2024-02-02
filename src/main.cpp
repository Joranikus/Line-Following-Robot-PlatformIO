
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

float direction(const bool sensor_activations[], int antall_sensor) {
    int lower_bound = antall_sensor + 1;
    int upper_bound = 0;

    for (int index = 0; index < antall_sensor; index++) {
        if (sensor_activations[index]) {
            if (index > upper_bound) {
                upper_bound = index;
            }

            if (index < lower_bound) {
                lower_bound = index;
            }
        }
    }

    // avg_bound vil nå være en float mellom 0 og sensor_activations.size()
    float avg_bound = static_cast<float>(lower_bound + upper_bound) / 2.0f;

    // avg_bound er nå mellom [0, 1]
    avg_bound /= static_cast<float>(antall_sensor - 1);

    // avg_bound går nå fra 0 til 300
    avg_bound = avg_bound * 300;

    return avg_bound;
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