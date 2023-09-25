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
void motorControl(double analogValue, int minValue, int maxValue, float speedAdjust) {
    // Map analog value within the given range to PWM range (0 to 255)
    int steer_value = map(analogValue, minValue, maxValue, 0, 255);
    //double steer_val_double = analogValue * 255;
    //int steer_value = static_cast<int>(steer_val_double);

    //Serial.println(steer_value);

    // Apply the speed adjustment
    if (steer_value > 128) {
        // Steer right
        leftSpeed = 255;
        rightSpeed = (255 - steer_value) * 2;
    } else if (steer_value < 128) {
        // Steer left
        leftSpeed = steer_value * 2;
        rightSpeed = 255;
    } else {
        // No steering, both motors forward
        leftSpeed = 255;
        rightSpeed = 255;
    }

    leftSpeed = leftSpeed * speedAdjust;
    rightSpeed = rightSpeed * speedAdjust;

    // Set motor speeds using PWM
    digitalWrite(motorSTBY, HIGH);

    analogWrite(motor1PWM, leftSpeed);
    digitalWrite(motor1AIN1, HIGH);
    digitalWrite(motor1AIN2, LOW);

    analogWrite(motor2PWM, rightSpeed);
    digitalWrite(motor2BIN1, HIGH);
    digitalWrite(motor2BIN2, LOW);
}

void PrintMotorSpeed(unsigned long interval, int leftSpeed, int rightSpeed, double inputRange)
{
    static unsigned long lastPrintTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastPrintTime >= interval)
    {
        //For loop timer
        //unsigned long dt = currentTime - lastPrintTime;

        lastPrintTime = currentTime;

        Serial.print("Analog Value: ");
        Serial.print(inputRange);
        Serial.print(" | Left Speed: ");
        Serial.print(leftSpeed);
        Serial.print(" | Right Speed: ");
        Serial.print(rightSpeed);

        /*Serial.print(" | loops / second: ");
        Serial.print(dt);
        Serial.print(" | Loops: ");
        Serial.println(iters);
        iters = 0;*/
    }
}

void loop()
{
    double dir = direction_class.get_direction();

    iters++;

    motorControl(dir, -1, 1, 1);
    PrintMotorSpeed(250, leftSpeed, rightSpeed, dir);
}