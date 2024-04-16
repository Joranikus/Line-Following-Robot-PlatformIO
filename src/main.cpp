
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "DirectionClass.hpp"
#include "MotorController.hpp"
#include "PID.hpp"
#include "Tests.hpp"
#include "BatteryManager.hpp"
#include "TimerStats.hpp"

int num_sensor_pins = 7;
int sensor_pins[7] = {17, 16, 5, 18, 21, 22, 23};

int yellow_light_pin = 15;
int green_led_pin = 2;
int red_led_pin = 4;
int voltage_pin = 35;

int sensor_lights_pin = 12;

DirectionClass direction_class{sensor_pins, num_sensor_pins};
MotorController motor_controller{0, 300};
PID pid{150, 2, 0.5, 0.0};
float pid_speed_adjust = 0.6;
float speed_adjust_90 = 0.6;
int turn_time = 1000;
Tests tests;
BatteryManager battery_manager{yellow_light_pin, green_led_pin, red_led_pin, voltage_pin};

void setup()
{
    ledcSetup(0, 300000, 8);
    ledcSetup(1, 300000, 8);

    ledcAttachPin(27, 0);
    ledcAttachPin(26, 1);

    battery_manager.set_battery_threshold(3.7, 3.2);

    //midlertidig sensorlys
    pinMode(sensor_lights_pin, OUTPUT);
    analogWrite(sensor_lights_pin, 4000);

    Serial.begin(115200);
    Serial.println();
    Serial.println("Setup complete.");

    WiFi.begin("NothingPhone", "Ok123456");

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }

    Serial.println("Wifi connected.");
    ArduinoOTA.begin();

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    ArduinoOTA.handle();

    if (!battery_manager.shutdown_status()) {
        battery_manager.update();
        Serial.println("Battery emtpy.");
        return;
    }

    battery_manager.update();

    if (direction_class.is_right_turn_detected() || direction_class.is_left_turn_detected()) {
        direction_class.execute_90_degree_turn(motor_controller, pid_speed_adjust, turn_time);
    } else {
        auto dir_without_pid = direction_class.get_direction();
        auto dir = MotorController::clamp(pid.output(dir_without_pid), 0, 300);
        motor_controller.motor_control_forward(dir, speed_adjust_90);
    }

    /////////////////////// TESTS ///////////////////////

    //tests.print_motor_speed(motor_controller, dir_without_pid, dir, 500);

    //tests.print_sensors(sensor_pins, num_sensor_pins, 500);

    //tests.print_status(battery_manager);

    //timerStats.startTimer();

    /////////////////////////////////////////////////////
}