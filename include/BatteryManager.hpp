
#ifndef LINE_FOLLOWING_ROBOT_PLATFORMIO_BATTERYMANAGER_HPP
#define LINE_FOLLOWING_ROBOT_PLATFORMIO_BATTERYMANAGER_HPP

class BatteryManager {

public:
    BatteryManager(int yellow_led_pin, int green_led_pin, int red_led_pin, int voltage_pin);
    void update();
    void set_battery_threshold(float yellow_threshold, float red_threshold);
    bool shutdown_status();
    bool get_yellow_led_status();
    bool get_green_led_status();
    bool get_red_led_status();
    float get_battery_voltage();


private:
    int yellow_led_pin;
    int green_led_pin;
    int red_led_pin;
    int voltage_pin;
    float battery_voltage;

    bool green_led_status;
    bool yellow_led_status;
    bool red_led_status;

    bool shutdown = false;

    float yellow_threshold;
    float red_threshold;
    void update_LEDs(); // Function to update LED status based on battery voltage
};

#endif //LINE_FOLLOWING_ROBOT_PLATFORMIO_BATTERYMANAGER_HPP
