//
// Created by oyste on 9/18/2023.
//

#include "DirectionClass.hpp"

namespace dir {
    DirectionClass::DirectionClass() {
        for (int i = 0; i < 100; i++)
        {
            past_directions[i] = 0.0;
        }
    }

    double DirectionClass::get_direction(int inPins[5]) {
        for (int i = 0; i < 99; i++)
        {
            past_directions[i] = past_directions[i + 1];
        }

        past_directions[99] = this_direction(inPins);

        double direction = 0.0;
        for (int i = 0; i < 100; i++)
        {
            direction += DirectionClass::weighted(100, i, past_directions[i]);
        }

        return direction;
    }

    double DirectionClass::weighted(int max, int ix, double value) {
        double num = ix * ix * ix * ix;
        double den = max * max * max * max;

        return value * (num / den);
    }

    int DirectionClass::arr_sum(int inPins[5])
    {
        int s = 0;
        for (int i = 0; i < 5; ++i) {
            s += inPins[i];
        }
        return s;
    }

    double DirectionClass::this_direction(int inPins[5]) {
        if (DirectionClass::arr_sum(inPins) == 0) {
            return 0.0;
        }
        else if (DirectionClass::arr_sum(inPins) == 5) {
            return 0.0;
        }

        int minIx = 5;
        int maxIx = -1;

        for (int i = 0; i < 5; i++)
        {
            if ((i > maxIx) and inPins[i]){maxIx = i;}
            if ((i < minIx) and inPins[i]){minIx = i;}
        }

        minIx -= 2;
        maxIx -= 2;

        double tmp = static_cast<double>(minIx) + (static_cast<double>(maxIx) - static_cast<double>(minIx)) / 2.0;
        return tmp;
    }

} // dir