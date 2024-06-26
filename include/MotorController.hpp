
#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP

class MotorController {
public:
    MotorController(double min_value, double max_value);
    void motor_control(double analog_value, double speed_adjust);
    static double clamp(double val, double min_value, double max_value);
    double get_left_speed();
    double get_right_speed();
    double get_steer_value();

private:
    void send_signal() const;

    int motor1PWM = 25;
    int motor2PWM = 27;

    double min_value = 0.0;
    double max_value = 300.0;

    double steer_value = 0;

    double left_speed = 0;
    double right_speed = 0;
};


#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP
