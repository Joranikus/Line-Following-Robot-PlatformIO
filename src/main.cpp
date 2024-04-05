
#include <Arduino.h>
#include "DirectionClass.hpp"
#include "MotorController.hpp"
#include "PID.hpp"
#include "Tests.hpp"
#include "BatteryManager.hpp"
#include "TimerStats.hpp"

int num_sensor_pins = 7;
int sensor_pins[7] = {22, 23, 16, 17, 5, 18, 21};

int yellow_light_pin = 15;
int green_led_pin = 2;
int red_led_pin = 4;
int voltage_pin = 35;

int sensor_lights_pin = 12;

DirectionClass direction_class{sensor_pins, num_sensor_pins};
MotorController motor_controller{0, 300};
PID pid{150, 1.0, 0.9, 0.0};
Tests tests;
BatteryManager battery_manager{yellow_light_pin, green_led_pin, red_led_pin, voltage_pin};
TimerStats timerStats;

void setup()
{
    ledcSetup(0, 300000, 8);
    ledcSetup(1, 300000, 8);

    ledcAttachPin(25, 0);
    ledcAttachPin(26, 1);

    battery_manager.set_battery_threshold(3.7, 3.2);
    timerStats = TimerStats();

    //midlertidig sensorlys
    pinMode(sensor_lights_pin, OUTPUT);
    analogWrite(sensor_lights_pin, 4000);

    Serial.begin(115200);
    Serial.println();
    Serial.println("Setup complete.");
}

void loop()
{
    if (!battery_manager.shutdown_status()) {
        Serial.println("Battery emtpy.");
        return;
    }

    battery_manager.update();

    //retrieves raw direction values
    auto dir_without_pid = direction_class.get_direction();

    //clamps PID output within specified range
    auto dir = MotorController::clamp(pid.output(dir_without_pid), 0, 300);

    //controls motors based on direction and speed adjustment
    motor_controller.motor_control(dir, 0.80);

    /////////////////////// TESTS ///////////////////////

    //tests.print_motor_speed(motor_controller, dir_without_pid, dir, 500);

    //tests.print_sensors(sensor_pins, num_sensor_pins, 500);

    //tests.print_status(battery_manager);

    /* START TIMER
    timerStats.startTimer();

    if (timerStats.printIters % 5000 == 0) {
        timerStats.printTimerData();
    } END TIMER
    */

    /////////////////////////////////////////////////////
}