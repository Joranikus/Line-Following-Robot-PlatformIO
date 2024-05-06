
#include <Arduino.h>
#include "DirectionClass.hpp"


DirectionClass::DirectionClass(std::array<short, 7> pins)
: numPins(pins.size()), sensor_pins(), out_pins() {
    /*Init a class to keep track of previous positions, and outputs new position.*/

    for (int i = 0; i < numPins; i++)
    {
        sensor_pins[i] = pins[i];
        out_pins[i] = 0;
    }
}

double DirectionClass::get_direction() {
    DirectionClass::read_sensor_pins();
    return DirectionClass::this_direction();
}

int DirectionClass::arraySum(std::array<short, 7> &values) {
    int sum = 0;
    for (int i : values) {
        sum += i;
    }
    return sum;
}

double DirectionClass::this_direction() {

    int as = arraySum(out_pins);
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
        out_pins[i] = static_cast<short>(digitalRead(sensor_pins[i]));
    }

    if (arraySum(out_pins) != 0) {
        for (int ix = 0; ix < 7; ix++) {
            prevActiveSensor[ix] = out_pins[ix];
        }
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

    auto left = prevActiveSensor[0];
    auto right = prevActiveSensor[6];
    auto edge = left or right;

    if (nullSensorIters > (3000 / 2.5) && edge) {
        extremeTurnActive = true;
    } else {
        extremeTurnActive = false;
    }
}

bool DirectionClass::waitForDetection(int maxTime) {

    // Will wait maxTime ms unless it can see the line.
    // Returns True if it finds the line, False otherwise.

    auto currentTime = millis();
    while (currentTime + maxTime > millis()) {
        read_sensor_pins();

        int sum = arraySum(out_pins);

        if (sum != 0 && sum != 7) {
            return true;
        }
    }

    return false;
}

void DirectionClass::execute_90_degree_turn(MotorController &motor_controller,
                                            float speed_adjust, int turn_time,
                                            unsigned long &last_detection_time,
                                            unsigned long cooldown_time) {
    auto current_time = millis();

    if (current_time - last_detection_time < cooldown_time) {
        Serial.println("Cooldown");
        return;
    }

    motor_controller.motor_control_forward(0, 0);

    bool left_detected = extremeTurnDirection == LEFT;
    bool right_detected = extremeTurnDirection == RIGHT;

    int back_time = 2000;

    if (left_detected || right_detected) {
        if (left_detected) {
            motor_controller.motor_control_backward(300, speed_adjust);
            waitForDetection(back_time);
            motor_controller.motor_control_left_turn(speed_adjust);
        } else {
            motor_controller.motor_control_backward(0, speed_adjust);
            waitForDetection(back_time);
            motor_controller.motor_control_right_turn(speed_adjust);
        }

        waitForDetection(turn_time);

    }

    last_detection_time = millis();
}