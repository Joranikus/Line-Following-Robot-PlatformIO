
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "DirectionClass.hpp"
#include "MotorController.hpp"
#include "PID.hpp"
#include "Tests.hpp"
#include "BatteryManager.hpp"

int num_sensor_pins = 7;
int sensor_pins[7] = {17, 16, 5, 18, 21, 22, 23};

int yellow_light_pin = 15;
int green_led_pin = 2;
int red_led_pin = 4;
int voltage_pin = 35;

int sensor_lights_pin = 12;

unsigned long last_detection_time = 0;
unsigned long cooldown_time = 1000;

DirectionClass direction_class{sensor_pins, num_sensor_pins};
MotorController motor_controller{0, 300};
PID pid{150, 1.4, 1, 0.1};
float pid_speed_adjust = 0.6;
float speed_adjust_90 = 0.7;
int turn_time = 0;
static unsigned long last_time = 0;

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

    auto startTime = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < 3000) {
        delay(100);
    }

    Serial.println("Wifi connected.");
    ArduinoOTA.begin();

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void loop()
{

    unsigned long current_time = micros();
    double dt = current_time - last_time;
    last_time = current_time;

    ArduinoOTA.handle();

    if (!battery_manager.shutdown_status()) {
        battery_manager.update();
        Serial.println("Battery emtpy.");
        return;
    }

    battery_manager.update();

    auto dir_without_pid = direction_class.get_direction();
    direction_class.updateExtremeTurn();

    auto dir = 0;
    if (direction_class.extremeTurnActive) {
        direction_class.execute_90_degree_turn(motor_controller, speed_adjust_90, turn_time, last_detection_time, cooldown_time);
        direction_class.extremeTurnActive = false;
    } else {
        dir = MotorController::clamp(pid.output(dir_without_pid, dt), 0, 300);
        motor_controller.motor_control_forward(dir, pid_speed_adjust);
    }
    //auto pid_output = pid.output(dir_without_pid, dt);
    //auto dir = MotorController::clamp(pid_output, 0, 300);

//    Serial.print(dir_without_pid);
//    Serial.print(" | ");
//    Serial.println(dir);

    //motor_controller.motor_control_forward(dir, pid_speed_adjust);

    /////////////////////// TESTS ///////////////////////

    tests.print_motor_speed(motor_controller, dir_without_pid, dir, 100);

    //tests.print_sensors(sensor_pins, num_sensor_pins, 500);

    //tests.print_status(battery_manager);

    /////////////////////////////////////////////////////
}