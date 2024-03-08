
#include <Arduino.h>
#include "DirectionClass.hpp"
#include "MotorController.hpp"
#include "PID.hpp"
#include "Tests.hpp"
#include "BatteryManager.hpp"

int num_sensor_pins = 7;
int sensor_pins[7] = {22, 23, 16, 17, 5, 18, 21};

int yellow_light_pin = 4;
int green_led_pin = 2;
int red_led_pin = 15;

DirectionClass direction_class{sensor_pins, num_sensor_pins};
MotorController motor_controller{0, 300};
PID pid{150, 1.0, 0.9, 0.0};
Tests tests;

void setup()
{
    ledcSetup(0, 300000, 8);
    ledcSetup(1, 300000, 8);

    ledcAttachPin(25, 0);
    ledcAttachPin(26, 1);

    Serial.begin(9600);
    Serial.println();
    Serial.println("Setup complete.");
}

void loop()
{
    //retrieves raw direction values
    auto dir_without_pid = direction_class.get_direction();

    //clamps PID output within specified range
    auto dir = MotorController::clamp(pid.output(dir_without_pid), 0, 300);

    //controls motors based on direction and speed adjustment
    motor_controller.motor_control(dir, 0.80);

    /////////////////////// TESTS ///////////////////////

    //tests.print_motor_speed(motor_controller, dir_without_pid, dir, 500);

    //tests.print_sensors(sensor_pins, num_sensor_pins, 500);

    /////////////////////////////////////////////////////
}