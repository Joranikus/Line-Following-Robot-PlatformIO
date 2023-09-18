//
// Created by oyste on 9/18/2023.
//

#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP


namespace dir {

    class DirectionClass {
    public:
        DirectionClass(int pins[5]);
        double get_direction(int inPins[5]);

    private:
        double past_directions[100];
        int arr_sum(int inPins[5]);
        double this_direction(int inPins[5]);
        double weighted(int max, int ix, double value);
        void update_past_directions(double value);


        int sensorPins[5];
        int sensorLimit = 500;
        void setSensorPinMode();
        int outPins[5];
        void readSensorPins();
    };

} // dir

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_DIRECTIONCLASS_HPP
