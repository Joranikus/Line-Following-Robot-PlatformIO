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
    }

    double DirectionClass::get_direction() {
        DirectionClass::readSensorPins();
        return DirectionClass::this_direction();
    }

    int DirectionClass::arr_sum(const int* arr, int size)
    {
        //Returns the sum of elements in array.
        //NB! Max length of array is 2^16, but max value of output is 2^15

        int sum = 0;
        for (unsigned int i = 0; i < size; ++i) {
            sum += arr[i];
        }
        return sum;
    }

    double DirectionClass::this_direction() {

        int as = arr_sum(outPins, antallPins);
        if ((as == 0) or (as == antallPins)) {return prev_direction;}

        //Sets the upper and lower bound to the edges.
        int minIx = antallPins;
        int maxIx = -1;

        //Updates the upper and lower bound based on pin state.
        for (int i = 0; i < antallPins; i++)
        {
            if ((i > maxIx) and outPins[i]){maxIx = i;}
            if ((i < minIx) and outPins[i]){minIx = i;}
        }

        auto avg_bound = static_cast<double>((minIx + maxIx)) / 2.0;
        auto normalized = avg_bound / static_cast<double>(antallPins - 1);
        auto shifted = normalized * 300.0;

        prev_direction = shifted;
        return shifted;
    }

    void DirectionClass::readSensorPins() {
        for (int i = 0; i < antallPins; i++)
        {
            outPins[i] = digitalRead(sensorPins[i]);
        }
    }


} // dir