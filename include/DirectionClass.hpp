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
        double past_directions[100] = {0};
        int length_of_past_dir = 100;

        static int arr_sum(const int* inPins, int size);
        double this_direction();
        static double weighted(int max, int ix, double value);
        void update_past_directions(double value);

        int antallPins;
        int sensorPins[5];
        int outPins[5];
        int sensorLimit = 500;
        void readSensorPins();

        double pid_SP = 0.0;
        double pid_Kp = 0.8;
        double pid_Ki = 0.0;
        double pid_Kd = 0.0;

        double prev_direction = 0.0;
        double prev_integral = 0.0;
        unsigned long prev_timestep = millis();
        unsigned long dt = 0.0;

        double get_proposional();
        double get_integral();
        double get_derived();
    };

} // dir

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
