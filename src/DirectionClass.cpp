//
// Created by oyste on 9/18/2023.
//

#include <Arduino.h>
#include "DirectionClass.hpp"

namespace dir {
    DirectionClass::DirectionClass(int pins[5]) {
        /*Init a class to keep track of previous positions, and outputs ned position.*/

        for (int i = 0; i < 5; i++)
        {
            sensorPins[i] = pins[i];
            outPins[i] = 0;
        }

        for (int i = 0; i < 100; i++)
        {
            past_directions[i] = 0.0;
        }
    }

    double DirectionClass::get_direction() {

        //Gets current direction from sensor, and updates the list of prev values.
        DirectionClass::readSensorPins();
        double new_value = DirectionClass::this_direction();
        DirectionClass::update_past_directions(new_value);

        //Calculates a new direction based on previous sensor inputs
        double direction = 0.0;
        for (int i = 0; i < 100; i++)
        {
            direction += DirectionClass::weighted(100, i, past_directions[i]);
        }

        return direction;
    }

    void DirectionClass::update_past_directions(double value) {
        //Shifts the whole list 1 spot, discarding the oldest value, and making room for the new one.
        for (int i = 0; i < 99; i++)
        {
            past_directions[i] = past_directions[i + 1];
        }
        past_directions[99] = value;
    }

    double DirectionClass::weighted(int max, int ix, double value) {
        //Returns a weighted value based on position in list.
        //Returns value * (ix ^ 4 / max ^ 4)

        double num = ix * ix * ix * ix;
        double den = max * max * max * max;

        return value * (num / den);
    }

    int DirectionClass::arr_sum(const int* arr)
    {
        //Returns the sum of elements in array.
        //NB! Max length of array is 2^16, but max value of output is 2^15

        int s = 0;
        for (unsigned int i = 0; i < sizeof(&arr) / sizeof(int); ++i) {
            s += arr[i];
        }
        return s;
    }

    double DirectionClass::this_direction() {

        int as = arr_sum(outPins);
        if ((as == 0) or (as == 5)){return 0.0;}

        //Sets the upper and lower bound to the edges.
        int minIx = 5;
        int maxIx = -1;

        //Updates the upper and lower bound based on pin state.
        for (int i = 0; i < 5; i++)
        {
            if ((i > maxIx) and outPins[i]){maxIx = i;}
            if ((i < minIx) and outPins[i]){minIx = i;}
        }

        //Shifts the bounds from [0, 4] to [-2, 2]
        minIx -= 2;
        maxIx -= 2;

        //Returns the lower bound plus half the difference. (Returns the number in the middle of the bounds)
        double tmp = static_cast<double>(minIx) + (static_cast<double>(maxIx) - static_cast<double>(minIx)) / 2.0;
        return tmp;
    }

    void DirectionClass::readSensorPins() {
        for (int i = 0; i < 5; i++)
        {
            outPins[i] = analogRead(sensorPins[i]) > sensorLimit;
        }
    }

} // dir