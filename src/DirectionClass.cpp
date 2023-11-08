//
// Created by oyste on 9/18/2023.
//

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

        outPins;
        if ;
        int left = 50;
        int right = 255;


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