#include <Arduino.h>

#include "DirectionClass.hpp"

//SETUP

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

int leftSpeed, rightSpeed;

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
void motorControl(double analogValue, double minVal, double maxVal, double speedAdjust) {
    double steer_value;

    if (minVal >= 0) {
        steer_value = analogValue - (maxVal - minVal) / 2.0;
    } else if ((-1 * minVal) != maxVal) {
        double diff = maxVal - minVal;
        steer_value = (analogValue - minVal) - diff / 2.0;
        maxVal = diff / 2.0;
        minVal = -diff / 2.0;
    } else {
        steer_value = analogValue;
    }

    double lS;
    double rS;

    if (steer_value > 0) {
        //Steer right
        lS = 255.0;
        rS = 255.0 - 255.0 * steer_value / maxVal;
    } else if (steer_value < 0) {
        //Steer left
        lS = 255.0 - 255.0 * steer_value / minVal;
        rS = 255.0;
    } else {
        lS = 255.0;
        rS = 255.0;
    }

    lS *= speedAdjust;
    rS *= speedAdjust;

    leftSpeed = static_cast<int>(lS);
    rightSpeed = static_cast<int>(rS);

    // Set motor speeds using PWM
    digitalWrite(motorSTBY, HIGH);

    analogWrite(motor1PWM, leftSpeed);
    digitalWrite(motor1AIN1, HIGH);
    digitalWrite(motor1AIN2, LOW);

    analogWrite(motor2PWM, rightSpeed);
    digitalWrite(motor2BIN1, HIGH);
    digitalWrite(motor2BIN2, LOW);
}

void PrintMotorSpeed(unsigned long interval, double inputRange)
{
    static unsigned long lastPrintTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastPrintTime >= interval)
    {
        lastPrintTime = currentTime;

        Serial.print("Analog Value: ");
        Serial.print(inputRange);
        Serial.print(" | Left Speed: ");
        Serial.print(leftSpeed);
        Serial.print(" | Right Speed: ");
        Serial.print(rightSpeed);

        Serial.println();
    }
}

void loop()
{
    double dir = direction_class.get_direction();

    motorControl(dir, -1, 1, 1);
    PrintMotorSpeed(250, dir);
}