
#include <Arduino.h>

int antallPins = 7;
int sensorPins[7] = {23, 22, 21, 20, 6, 4, 3};
float prev_dir;

int motorPins[2] = {9, 10};

#define motor1PWM motorPins[0]      // PWM Left Motor
#define motor2PWM motorPins[1]      // PWM Right Motor

int leftSpeed, rightSpeed, steer_value_global;

void setup()
{
    // Activates serial
    Serial.begin(9600);

    // Motor Output
    for (int pin : motorPins)
    {
        pinMode(pin, OUTPUT);
    }

    //Sensor
    for (int pin : sensorPins)
    {
        pinMode(pin, INPUT);
    }

}

//MOTOR CODE
// Function to control motors based on analog input and its range
void motorControl(double analogValue, double minValue, double maxValue, double speedAdjust) {
    // Map analog value within the given range to PWM range (0 to 255)
    //int steer_value = map(analogValue, minValue, maxValue, 0, 255);


    double analog = max(minValue, min(analogValue, maxValue));
    int steer_value = (analog - minValue) / (maxValue - minValue) * 255;
    steer_value_global = steer_value;
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
    analogWrite(motor1PWM, leftSpeed);
    analogWrite(motor2PWM, rightSpeed);
}

void PrintMotorSpeed(unsigned long interval, int leftSpeed, int rightSpeed, double inputRange, double loopTime)
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
        Serial.print(" | Steer value: ");
        Serial.print(steer_value_global);
        Serial.print(" | Looptime: ");
        Serial.print(" loopTime");

        Serial.println();
    }
}

float direction(const bool sensor_activations[], int antall_sensor) {
    int lower_bound = antall_sensor + 1;
    int upper_bound = 0;

    for (int index = 0; index < antall_sensor; index++) {
        if (sensor_activations[index]) {
            if (index > upper_bound) {
                upper_bound = index;
            }

            if (index < lower_bound) {
                lower_bound = index;
            }
        }
    }

    // avg_bound vil nå være en float mellom 0 og sensor_activations.size()
    float avg_bound = static_cast<float>(lower_bound + upper_bound) / 2.0f;

    // avg_bound er nå mellom [0, 1]
    avg_bound /= static_cast<float>(antall_sensor - 1);

    // avg_bound går nå fra -1 til 1
    avg_bound = avg_bound * 2 - 1;

    return avg_bound;
}

void loop()
{
    auto startTime = millis();

    bool sensor_activations[7];
    int sum = 0;
    for (int i = 0; i < antallPins; i++)
    {
        sensor_activations[i] = digitalRead(sensorPins[i]);
        sum += sensor_activations[i];
    }

    float dir;
    if ((sum == 0) || (sum == antallPins)) {
        dir = prev_dir;
    } else {
        dir = direction(sensor_activations, antallPins);
        prev_dir = dir;
    }

    motorControl(dir, -1.0, 1.0, 0.7);

    auto endTime = millis();
    PrintMotorSpeed(250, leftSpeed, rightSpeed, dir, endTime - startTime);
}