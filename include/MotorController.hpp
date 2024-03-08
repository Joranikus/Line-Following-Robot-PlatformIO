
#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP

class MotorController {
public:
    MotorController(double min_value, double max_value);
    void motor_control(double analog_value, double speed_adjust);
    void print_motor_speed(double current_direction, double loop_time, double dir_without_pid) const;
    static double clamp(double val, double min_value, double max_value);

private:
    void send_signal() const;

    int motor1PWM = 25;
    int motor2PWM = 26;

    double min_value = 0.0;
    double max_value = 300.0;

    double steer_value = 0;

    int print_interval = 1000;
    double left_speed = 0;
    double right_speed = 0;
};


#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_MOTORCONTROLLER_HPP
