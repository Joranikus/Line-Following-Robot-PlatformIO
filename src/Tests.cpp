
#include <Arduino.h>
#include "Tests.hpp"

void Tests::print_sensors(const int *pins, const int num_pins) {
    for (int ix = 0; ix < num_pins; ix++)
    {
        Serial.print(digitalRead(pins[ix]));

        if (ix + 1 < num_pins) {
            Serial.print("|");
        }
    }

    Serial.println();
}