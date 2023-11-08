#include <Arduino.h>

#include "DirectionClass.hpp"

//SETUP

//For loop timer
//int iters = 0;

int sensorPins[5] = {
        A0, // Sensor 1
        A1, // Sensor 2
        A2, // Sensor 3
        A3, // Sensor 4
        A4, // Sensor 5
};

//Creates a direction object.
dir::DirectionClass direction_class{sensorPins, 5};

int motorPins[7] = {5, 2, 3, 6, 7, 8, 9};

#define motor1PWM motorPins[0]      // PWM Left Motor
#define motor1AIN1 motorPins[1]     // AIN1 Left Motor
#define motor1AIN2 motorPins[2]    // AIN2 Left Motor
#define motor2PWM motorPins[3]      // PWM Right Motor
#define motor2BIN1 motorPins[4]     // AIN1 Right Motor
#define motor2BIN2 motorPins[5]     // AIN2 Right Motor
#define motorSTBY motorPins[6]      // STBY (HIGH = Driver ON) (LOW = Driver OFF)

//int leftSpeed, rightSpeed, steer_value_global;

void setup()
{
    // Activates serial
    Serial.begin(9600);

    // Motor Output
    for (int pin : motorPins)
    {
        pinMode(pin, OUTPUT);
    }
}

//MOTOR CODE

// Function to control motors based on analog input and its range
void motorControl(dir::Tuple dir) {

    // Set motor speeds using PWM
    //digitalWrite(motorSTBY, HIGH);

    analogWrite(motor1PWM, dir.left);
    //digitalWrite(motor1AIN1, HIGH);
    //digitalWrite(motor1AIN2, LOW);

    analogWrite(motor2PWM, dir.right);
    //digitalWrite(motor2BIN1, HIGH);
    //digitalWrite(motor2BIN2, LOW);
}

void PrintMotorSpeed(unsigned long interval, dir::Tuple dir)
{
    static unsigned long lastPrintTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastPrintTime >= interval)
    {
        //For loop timer
        //unsigned long dt = currentTime - lastPrintTime;

        lastPrintTime = currentTime;

        Serial.print(" | Left Speed: ");
        Serial.print(dir.left);
        Serial.print(" | Right Speed: ");
        Serial.print(dir.right);

        Serial.println();

        /*Serial.print(" | loops / second: ");
        Serial.print(dt);
        Serial.print(" | Loops: ");
        Serial.println(iters);
        iters = 0;*/
    }
}

void loop()
{
    dir::Tuple dir = direction_class.get_direction();


    //For loop timer
    //iters++;

    motorControl(dir);
    PrintMotorSpeed(250, dir);
}