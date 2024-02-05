//
// Created by Øystein Bringsli.
//

#include <Arduino.h>

#include "Tests.hpp"

void Tests::printSensors(const int *pins, const int numPins) {
    for (int ix = 0; ix < numPins; ix++)
    {
        //Pin 4 is turned off!!!!

        Serial.print(digitalRead(pins[ix]));

        if (ix + 1 < numPins) {
            Serial.print("|");
        }

        //Pin 4 is turned off!!!!
    }

    Serial.println();
}