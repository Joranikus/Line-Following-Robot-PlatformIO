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

    double DirectionClass::get_direction() {

        //Gets current direction from sensor, and updates the list of prev values.
        DirectionClass::readSensorPins();
        double new_value = DirectionClass::this_direction();
        DirectionClass::update_past_directions(new_value, lenght_of_past_dir);

        //Calculates a new direction based on previous sensor inputs
        double direction = 0.0;
        for (int i = 0; i < lenght_of_past_dir; i++)
        {
            direction += DirectionClass::weighted(100, i, past_directions[i]);
        }

        double PID_value = get_proposional() + get_integral() + get_integral();
        return direction + PID_value;
    }

    void DirectionClass::update_past_directions(double value, int length_of_prev_direction_list) {
        //Shifts the whole list 1 spot, discarding the oldest value, and making room for the new one.
        for (int i = 0; i < (length_of_prev_direction_list - 1); i++)
        {
            past_directions[i] = past_directions[i + 1];
        }
        past_directions[length_of_prev_direction_list - 1] = value;
    }

    double DirectionClass::weighted(int max, int ix, double value) {
        //Returns a weighted value based on position in list.
        //Returns value * (ix ^ 4 / max ^ 4)

        double num = ix * ix * ix * ix;
        double den = max * max * max * max;

        return value * (num / den);
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
        if ((as == 0) or (as == antallPins)){return 0.0;}

        //Sets the upper and lower bound to the edges.
        int minIx = antallPins;
        int maxIx = -1;

        //Updates the upper and lower bound based on pin state.
        for (int i = 0; i < antallPins; i++)
        {
            if ((i > maxIx) and outPins[i]){maxIx = i;}
            if ((i < minIx) and outPins[i]){minIx = i;}
        }

        //Shifts the bounds from [0, antallPins - 1] to [0, 1]
        double minIxDouble = static_cast<double>(minIx) / static_cast<double>(antallPins);
        double maxIxDouble = static_cast<double>(maxIx) / static_cast<double>(antallPins);

        //Returns the lower bound plus half the difference. (Returns the number in the middle of the bounds)
        double tmp = minIxDouble + (maxIxDouble - minIxDouble) / 2.0;
        return tmp;
    }

    void DirectionClass::readSensorPins() {
        for (int i = 0; i < antallPins; i++)
        {
            outPins[i] = analogRead(sensorPins[i]) > sensorLimit;
        }
    }

    double DirectionClass::get_proposional() {
        return pid_Kp * past_directions[lenght_of_past_dir - 1];
    }

    double DirectionClass::get_integral() {
        if (past_directions[lenght_of_past_dir - 1] * prev_integral < 0) {prev_integral = 0;}

        prev_integral += pid_Ki * past_directions[lenght_of_past_dir - 1];
        return prev_integral;

//        double sum = 0;
//        for (double val : past_directions)
//        {
//            if (sum * val < 0){sum = 0;}
//            else {sum += val;}
//        }
//
//        return pid_Ki * sum;
    }

} // dir