
#include <Arduino.h>
#include "DirectionClass.hpp"

DirectionClass::DirectionClass(const int *pins, int antall_pins)
: antall_pins(antall_pins), sensor_pins(), out_pins() {
    /*Init a class to keep track of previous positions, and outputs new position.*/

    for (int i = 0; i < antall_pins; i++)
    {
        sensor_pins[i] = pins[i];
        out_pins[i] = 0;
    }
}

double DirectionClass::get_direction() {
    DirectionClass::read_sensor_pins();
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

    int as = arr_sum(out_pins, antall_pins);
    if ((as == 0) or (as == antall_pins)) {return prev_direction;}

    //Sets the upper and lower bound to the edges.
    int minIx = antall_pins + 1;
    int maxIx = -1;

    //Updates the upper and lower bound based on pin state.
    for (int i = 0; i < antall_pins; i++)
    {
        if ((i > maxIx) and out_pins[i]){ maxIx = i;}
        if ((i < minIx) and out_pins[i]){ minIx = i;}
    }

    auto avg_bound = static_cast<double>((minIx + maxIx)) / 2.0;
    auto normalized = avg_bound / static_cast<double>(antall_pins - 1);
    auto shifted = normalized * 300.0;

    prev_direction = shifted;
    return shifted;
}

void DirectionClass::read_sensor_pins() {
    for (int i = 0; i < antall_pins; i++)
    {
        out_pins[i] = digitalRead(sensor_pins[i]);
    }
}
