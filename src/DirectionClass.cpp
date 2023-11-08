
#include <Arduino.h>
#include "DirectionClass.hpp"

namespace dir {
    DirectionClass::DirectionClass(const int pins[], int antallPins)
    : antallPins(antallPins) {
        /*Init a class to keep track of previous positions, and outputs new position.*/

        for (int i = 0; i < antallPins; i++)
        {
            sensorPins[i] = pins[i];
            outPins[i] = 0;
        }

        for (int pin : sensorPins)
        {
            pinMode(pin, INPUT);
        }
    }

    Tuple DirectionClass::get_direction() {

        //Gets current direction from sensor, and updates the list of prev values.
        DirectionClass::readSensorPins();

        bool bigLeft = outPins[4];
        bool smallLeft = outPins[3];
        bool front = outPins[2];
        bool smallRight = outPins[1];
        bool bigRight = outPins[0];

        if (bigLeft) {
            int left = 70;
            int right = 255;
        }

        if (smallLeft) {
            int left = 150;
            int right = 255;
        }

        if (front) {
            int left = 255;
            int right = 255;
        }

        if (smallRight) {
            int left = 255;
            int right = 150;
        }

        if (bigRight) {
            int left = 255;
            int right = 70;
        }
        return {left, right};
    }

    void DirectionClass::readSensorPins() {

        //Serial.println(analogRead(sensorPins[0]));
        for (int i = 0; i < antallPins; i++)
        {
            outPins[i] = analogRead(sensorPins[i]) > sensorLimit;
        }
    }
} // dir