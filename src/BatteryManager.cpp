
#include "BatteryManager.hpp"

BatteryManager::BatteryManager(int yellow_led_pin, int green_led_pin, int red_led_pin)
        : antall_pins(antall_pins), sensor_pins(), out_pins() {}