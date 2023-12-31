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

        prev_timestep = millis();
    }

    double DirectionClass::get_direction() {

        //Gets current direction from sensor, and updates the list of prev values.
        DirectionClass::readSensorPins();
        double new_value = DirectionClass::this_direction();
        DirectionClass::update_past_directions(new_value);

        //return past_directions[length_of_past_dir - 1];

        unsigned long new_time = millis();
        dt = new_time - prev_timestep;
        prev_timestep = new_time;


        //Propsional vil være Kp * [-1 til 1]
        //=> PID_value vil ikke alltid vil være mellom -1 og 1
        //Dermed må den på ett eller anna vis bli normalisert
        //Så vi faktisk kan plusse på integral og derived.
        //Eks: proposional = 1, integral = 1 => PID_value = 2

        double prop_part = get_proposional();
        double inte_part = get_integral();
        double derv_part = get_derived();

        double PID_value = prop_part + inte_part + derv_part;
        //Serial.print("Prop: ");
        //Serial.print(prop_part);
        //Serial.print(" | Inte: ");
        //Serial.print(inte_part);
        //Serial.print(" | Derv: ");
        //Serial.print(derv_part);
        //Serial.print(" | Tot: ");
        //Serial.println(PID_value);

        return PID_value;
    }

    void DirectionClass::update_past_directions(double value) {
        //Shifts the whole list 1 spot, discarding the oldest value, and making room for the new one.
        for (int i = 0; i < (length_of_past_dir - 1); i++)
        {
            past_directions[i] = past_directions[i + 1];
        }
        past_directions[length_of_past_dir - 1] = value;
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
        if ((as == 0) or (as == antallPins)){return prev_direction;}

        //Sets the upper and lower bound to the edges.
        int minIx = antallPins;
        int maxIx = -1;

        //Updates the upper and lower bound based on pin state.
        for (int i = 0; i < antallPins; i++)
        {
            if ((i > maxIx) and outPins[i]){maxIx = i;}
            if ((i < minIx) and outPins[i]){minIx = i;}
        }

        //Shifts the bounds from [0, antallPins - 1] to [0, max_val] => [-1, 1]
        double max_val = 2.0;
        double minIxDouble = max_val * static_cast<double>(minIx) / (static_cast<double>(antallPins) - 1.0) - 1.0;
        double maxIxDouble = max_val * static_cast<double>(maxIx) / (static_cast<double>(antallPins) - 1.0) - 1.0;

        //Returns the lower bound plus half the difference. (Returns the number in the middle of the bounds)
        double tmp = minIxDouble + (maxIxDouble - minIxDouble) / 2.0;

        prev_direction = tmp;
        return tmp;
    }

    void DirectionClass::readSensorPins() {

        //Serial.println(analogRead(sensorPins[0]));
        for (int i = 0; i < antallPins; i++)
        {
            outPins[i] = analogRead(sensorPins[i]) > sensorLimit;
        }
    }

    double DirectionClass::get_proposional() {
        return pid_Kp * (past_directions[length_of_past_dir - 1] - pid_SP);
    }

    double DirectionClass::get_integral() {
        if ((past_directions[length_of_past_dir - 1] - pid_SP) * prev_integral < 0) {prev_integral = 0;}
        if ((past_directions[length_of_past_dir - 1] - pid_SP) == 0) {prev_integral = 0;}

        prev_integral += pid_Ki * (past_directions[length_of_past_dir - 1] - pid_SP) * dt;
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

    double DirectionClass::get_derived() {
        double out = (past_directions[length_of_past_dir - 1]
                - past_directions[length_of_past_dir - 2])
                        / dt;

        return pid_Kd * out;
    }

} // dir