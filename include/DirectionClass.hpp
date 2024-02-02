//
// Created by oyste on 9/18/2023.
//

#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP

#include <Arduino.h>

namespace dir {

    class DirectionClass {
    public:
        explicit DirectionClass(const int *pins, int antallPins);
        double get_direction();

    private:

        static int arr_sum(const int* inPins, int size);
        double this_direction();

        int antallPins;
        int sensorPins[7];
        int outPins[7];
        void readSensorPins();
        double prev_direction = 0.0;
    };

} // dir

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
