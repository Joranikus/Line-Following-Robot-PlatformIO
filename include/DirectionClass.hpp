
#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP

#include <Arduino.h>

namespace dir {

    struct Tuple{
        int left;
        int right;
    };

    class DirectionClass {
    public:
        explicit DirectionClass(const int *pins, int antallPins);
        Tuple get_direction();

    private:
        int antallPins;
        int sensorPins[5];
        int outPins[5];
        int sensorLimit = 500;
        void readSensorPins();
    };

} // dir

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
