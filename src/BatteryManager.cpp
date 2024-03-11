#include "Arduino.h"
#include "BatteryManager.hpp"

BatteryManager::BatteryManager(int yellow_led_pin, int green_led_pin, int red_led_pin, int voltage_pin)
        : yellow_led_pin(yellow_led_pin), green_led_pin(green_led_pin),
          red_led_pin(red_led_pin), voltage_pin(voltage_pin) {
    pinMode(yellow_led_pin, OUTPUT);
    pinMode(green_led_pin, OUTPUT);
    pinMode(red_led_pin, OUTPUT);
}

void BatteryManager::update() {
    float voltage_divider_ratio = 0.5;
    this->battery_voltage = float((analogRead(voltage_pin) * (3.3 / 4095)) / voltage_divider_ratio);
    update_LEDs();
}

void BatteryManager::set_battery_threshold(float yellow_threshold, float red_threshold) {
    this->yellow_threshold = yellow_threshold;
    this->red_threshold = red_threshold;
}

bool BatteryManager::shutdown_status() {
    if (this->shutdown) {
        return false;
    } else {
        return true;
    }
}

void BatteryManager::update_LEDs() {
    if (battery_voltage <= red_threshold) {
        this->shutdown = true;
        digitalWrite(red_led_pin, 1);
        digitalWrite(yellow_led_pin, 0);
        digitalWrite(green_led_pin, 0);
        red_led_status = true;
        yellow_led_status = false;
        green_led_status = false;
    } else if (battery_voltage <= yellow_threshold) {
        digitalWrite(red_led_pin, 0);
        digitalWrite(yellow_led_pin, 1);
        digitalWrite(green_led_pin, 0);
        red_led_status = false;
        yellow_led_status = true;
        green_led_status = false;
    } else {
        digitalWrite(red_led_pin, 0);
        digitalWrite(yellow_led_pin, 0);
        digitalWrite(green_led_pin, 1);
        red_led_status = false;
        yellow_led_status = false;
        green_led_status = true;
    }
}

bool BatteryManager::get_yellow_led_status() {
    return yellow_led_status;
}

bool BatteryManager::get_green_led_status() {
    return green_led_status;
}

bool BatteryManager::get_red_led_status() {
    return red_led_status;
}

float BatteryManager::get_battery_voltage() {
    return battery_voltage;
}
