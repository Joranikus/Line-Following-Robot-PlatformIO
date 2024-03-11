#include <Arduino.h>
#include "Tests.hpp"
#include "MotorController.hpp"
#include "BatteryManager.hpp"

void Tests::print_sensors(int *pins, int num_pins, int print_delay) const {
    static unsigned long last_print_time = 0;
    unsigned long current_time = millis();

    // Check if enough time has elapsed since the last print
    if (current_time - last_print_time >= print_delay) {
        for (int ix = 0; ix < num_pins; ix++)
        {
            Serial.print(digitalRead(pins[ix]));

            if (ix + 1 < num_pins) {
                Serial.print("|");
            }
        }

        Serial.println();

        // Update the last print time
        last_print_time = current_time;
    }
}

void Tests::print_motor_speed(MotorController& motor_controller, double dir_without_pid, double current_direction, int print_delay) const {
    static unsigned long last_print_time = 0;
    unsigned long current_time = millis();

    // Check if enough time has elapsed since the last print
    if (current_time - last_print_time >= print_delay) {
        Serial.print("Analog Value: ");
        Serial.print(current_direction);
        Serial.print(" | Without pid: ");
        Serial.print(dir_without_pid);
        Serial.print(" | Left Speed: ");
        Serial.print(motor_controller.get_left_speed());
        Serial.print(" | Right Speed: ");
        Serial.print(motor_controller.get_right_speed());
        Serial.print(" | Steer value: ");
        Serial.print(motor_controller.get_steer_value());
        Serial.println();

        // Update the last print time
        last_print_time = current_time;
    }
}

void Tests::print_status(BatteryManager& battery_manager) {
    Serial.print("GREEN LED: ");
    Serial.print(battery_manager.get_green_led_status());
    Serial.print(" | YELLOW LED: ");
    Serial.print(battery_manager.get_yellow_led_status());
    Serial.print(" | RED LED: ");
    Serial.print(battery_manager.get_red_led_status());
    Serial.print(" | VOLTAGE: ");
    Serial.print(battery_manager.get_battery_voltage());
    Serial.println();
}
