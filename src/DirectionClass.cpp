
#include <Arduino.h>
#include "DirectionClass.hpp"


DirectionClass::DirectionClass(const int *pins, int antall_pins)
: numPins(antall_pins), sensor_pins(), out_pins() {
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

    int as = arr_sum(out_pins, numPins);
    if ((as == 0) or (as == numPins)) {return prev_direction;}

    //Sets the upper and lower bound to the edges.
    int minIx = numPins + 1;
    int maxIx = -1;

    //Updates the upper and lower bound based on pin state.
    for (int i = 0; i < numPins; i++)
    {
        if ((i > maxIx) and out_pins[i]){ maxIx = i;}
        if ((i < minIx) and out_pins[i]){ minIx = i;}
    }

    auto avg_bound = static_cast<double>((minIx + maxIx)) / 2.0;
    auto normalized = avg_bound / static_cast<double>(numPins - 1);
    auto shifted = normalized * 300.0;

    prev_direction = shifted;
    return shifted;
}

void DirectionClass::read_sensor_pins() {
    for (int ix = 0; ix < 7; ix++) {
        prevSensor[ix] = out_pins[ix];
    }

    for (int i = 0; i < numPins; i++)
    {
        out_pins[i] = digitalRead(sensor_pins[i]);
    }
}

void DirectionClass::updateExtremeTurn() {
    if (out_pins[0] && !out_pins[6]) {
        extremeTurnDirection = LEFT;
    } else if (out_pins[6] && !out_pins[0]) {
        extremeTurnDirection = RIGHT;
    }

    int sum = 0;
    for (int out_pin : out_pins) {
        sum += out_pin;
    }

    if (sum == 0) {
        nullSensorIters++;
    } else {
        nullSensorIters = 0;
    }

    if (nullSensorIters > 20) {
        extremeTurnActive = true;
    } else {
        extremeTurnActive = false;
    }

    Serial.print("Iters: ");
    Serial.print(nullSensorIters);
    Serial.print(" | ExtremeTurnActive: ");
    Serial.println(extremeTurnActive);
    return;

    auto left = is_left_turn_detected();
    auto right = is_right_turn_detected();

    if (left && right) {
        extremeTurnDirection = OFF;
    } else if (left) {
        extremeTurnDirection = LEFT;
    } else if (right) {
        extremeTurnDirection = RIGHT;
    } else {
        extremeTurnDirection = OFF;
    }
}

bool DirectionClass::is_left_turn_detected() {
    auto sum = arr_sum(out_pins, 7);

    if (sum > 1) {
        Serial.println("From sum > 1");
        return false;
    }

    if (out_pins[0]) {
        Serial.println("From outPins[0]");
        return true;
    }

    if (sum != 0) {
        Serial.println("From sum != 0");
        return false;
    }

    int sumPrev = 0;
    for (auto x : prevSensor) {
        sumPrev += x;
    }

    if (sumPrev == 0 && extremeTurnDirection == LEFT) {
        Serial.println("From sumPrev && ..");
        return true;
    }

    if (sumPrev > 2) {
        Serial.println("From sumPrev > 2");
        return false;
    }

    if (prevSensor[0] || prevSensor[1]) {
        Serial.println("From prev || prev");
        return true;
    }

    Serial.println("From default");
    return false;
}

bool DirectionClass::is_right_turn_detected() {
    auto sum = arr_sum(out_pins, 7);
    if (sum > 1) {
        Serial.println("From sum > 1");
        return false;
    }

    if (out_pins[6]) {
        Serial.println("From outPins[6]");
        return true;
    }

    if (sum != 0) {
        Serial.println("From sum != 0");
        return false;
    }

    int sumPrev = 0;
    for (auto x : prevSensor) {
        sumPrev += x;
    }

    if (sumPrev == 0 && extremeTurnDirection == LEFT) {
        Serial.println("From sumPrev == 0 && ...");
        return true;
    }

    if (sumPrev > 2) {
        Serial.println("From sumPrev > 2");
        return false;
    }

    if (prevSensor[6] || prevSensor[5]) {
        Serial.println("From prev || prev");
        return true;
    }

    Serial.println("From defoult");
    return false;
}

/*bool DirectionClass::is_left_turn_detected() {
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
}*/

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

    bool left_detected = extremeTurnDirection == LEFT;
    bool right_detected = extremeTurnDirection == RIGHT;

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