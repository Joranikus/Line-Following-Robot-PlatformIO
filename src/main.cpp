
#include <Arduino.h>
#include "DirectionClass.hpp"
#include "MotorController.hpp"
#include "PID.hpp"
#include "Tests.hpp"

int num_sensor_pins = 7;
int sensor_pins[7] = {22, 23, 16, 17, 5, 18, 21};

DirectionClass directionClass{sensor_pins, num_sensor_pins};
MotorController motorController{0, 300};
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

unsigned long last_print_time = 0;

void loop()
{
    auto start_time = millis();

    //retrieves raw direction values
    auto dir_without_pid = directionClass.get_direction();

    //clamps PID output within specified range
    auto dir = MotorController::clamp(pid.output(dir_without_pid), 0, 300);

    //controls motors based on direction and speed adjustment
    motorController.motor_control(dir, 0.80);

    auto end_time = millis();

    //prints motor speed
    motorController.print_motor_speed(dir, end_time - start_time, dir_without_pid);

    auto current_time = millis();

    //prints sensor values
    if (current_time > (last_print_time + 500)) {
        tests.print_sensors(sensor_pins, num_sensor_pins);
        last_print_time = current_time;
    }
}