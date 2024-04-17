
#include <Arduino.h>
#include "DirectionClass.hpp"


DirectionClass::DirectionClass(const int *pins, int antall_pins)
: antall_pins(antall_pins), sensor_pins(), out_pins() {
    /*Init a class to keep track of previous positions, and outputs new position.*/

    for (int i = 0; i < antall_pins; i++)
    {
        sensor_pins[i] = pins[i];
        out_pins[i] = 0;
    }
}

double DirectionClass::get_direction() {
    DirectionClass::read_sensor_pins();
    return DirectionClass::this_direction();
}

int DirectionClass::arr_sum(const int* arr, int size)
{
    //Returns the sum of elements in array.
    //NB! Max length of array is 2^16, but max value of output is 2^15

    int sum = 0;
    for (unsigned int i = 0; i < size; ++i) {
        sum += arr[i];
    }
    return sum;
}

double DirectionClass::this_direction() {

    int as = arr_sum(out_pins, antall_pins);
    if ((as == 0) or (as == antall_pins)) {return prev_direction;}

    //Sets the upper and lower bound to the edges.
    int minIx = antall_pins + 1;
    int maxIx = -1;

    //Updates the upper and lower bound based on pin state.
    for (int i = 0; i < antall_pins; i++)
    {
        if ((i > maxIx) and out_pins[i]){ maxIx = i;}
        if ((i < minIx) and out_pins[i]){ minIx = i;}
    }

    auto avg_bound = static_cast<double>((minIx + maxIx)) / 2.0;
    auto normalized = avg_bound / static_cast<double>(antall_pins - 1);
    auto shifted = normalized * 300.0;

    prev_direction = shifted;
    return shifted;
}

void DirectionClass::read_sensor_pins() {
    for (int i = 0; i < antall_pins; i++)
    {
        out_pins[i] = digitalRead(sensor_pins[i]);
    }
}
/*
bool DirectionClass::is_left_turn_detected() {
    unsigned long start_time = millis();
    bool detected[3] = {false}; // Array to track detection of each sensor
    while (millis() - start_time < 100) {
        read_sensor_pins();

        for (int i = 0; i < 3; ++i) {
            if (out_pins[i] == HIGH) {
                detected[i] = true; // Mark sensor as detected if HIGH
            }
        }

        // Check if all sensors are detected
        bool all_detected = true;
        for (int i = 0; i < 3; ++i) {
            if (!detected[i]) {
                all_detected = false; // If any sensor is not detected, break
                break;
            }
        }
        if (all_detected) {
            Serial.println("left turn detected");

            return true; // Return true if all sensors are detected
        }
        delay(10); // Small delay for smoother loop execution
    }
    return false;
}

bool DirectionClass::is_right_turn_detected() {
    unsigned long start_time = millis();
    bool detected[3] = {false}; // Array to track detection of each sensor
    while (millis() - start_time < 100) {
        read_sensor_pins();

        for (int i = antall_pins - 3; i < antall_pins; ++i) {
            if (out_pins[i] == HIGH) {
                detected[i - (antall_pins - 3)] = true; // Mark sensor as detected if HIGH
            }
        }

        // Check if all sensors are detected
        bool all_detected = true;
        for (int i = 0; i < 3; ++i) {
            if (!detected[i]) {
                all_detected = false; // If any sensor is not detected, break
                break;
            }
        }
        if (all_detected) {
            Serial.println("right turn detected");
            return true; // Return true if all sensors are detected
        }
        delay(10); // Small delay for smoother loop execution
    }
    return false;
}
*/

bool DirectionClass::is_left_turn_detected() {
    bool detected_sensor_0 = false;
    bool detected_sensor_2 = false;

        read_sensor_pins();

        if (out_pins[0] == HIGH) {
            detected_sensor_0 = true;
        }
        if (out_pins[1] == HIGH) {
            detected_sensor_2 = true;
        }

        // Check if both sensors 0 and 2 are detected
        if (detected_sensor_0 && detected_sensor_2) {
            Serial.println("Left turn detected");
            return true;
        }

    return false;
}

bool DirectionClass::is_right_turn_detected() {
    bool detected_sensor_4 = false;
    bool detected_sensor_6 = false;

        read_sensor_pins();

        if (out_pins[5] == HIGH) {
            detected_sensor_4 = true;
        }
        if (out_pins[6] == HIGH) {
            detected_sensor_6 = true;
        }

        // Check if both sensors 4 and 6 are detected
        if (detected_sensor_4 && detected_sensor_6) {
            Serial.println("Right turn detected");
            return true;
        }

    return false;
}

void DirectionClass::execute_90_degree_turn(MotorController &motor_controller,
                                            float speed_adjust, int turn_time,
                                            unsigned long &last_detection_time,
                                            unsigned long cooldown_time) {
    unsigned long current_time = millis();

    if (current_time - last_detection_time < cooldown_time) {
        Serial.println("Cooldown");
        return;
    }

    motor_controller.motor_control_forward(0, 0);

    bool left_detected = is_left_turn_detected();
    bool right_detected = is_right_turn_detected();

    if (left_detected || right_detected) {
        if (left_detected) {
            motor_controller.motor_control_left_turn(speed_adjust);
        } else {
            motor_controller.motor_control_right_turn(speed_adjust);
        }
        delay(turn_time);
        motor_controller.motor_control_forward(0, 0);
    }

    last_detection_time = millis();
}