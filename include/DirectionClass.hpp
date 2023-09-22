//
// Created by oyste on 9/18/2023.
//

#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP


namespace dir {

    class DirectionClass {
    public:
        explicit DirectionClass(const int *pins, int antallPins);
        double get_direction();

    private:
        double past_directions[100] = {0};
        int lenght_of_past_dir = 100;

        static int arr_sum(const int* inPins, int size);
        double this_direction();
        static double weighted(int max, int ix, double value);
        void update_past_directions(double value, int length_of_prev_direction_list);

        int antallPins;
        int sensorPins[5];
        int outPins[5];
        int sensorLimit = 500;
        void readSensorPins();
    };

} // dir

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
