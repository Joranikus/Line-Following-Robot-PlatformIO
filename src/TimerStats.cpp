//
// Created by Øystein Bringsli.
//

#include <Arduino.h>
#include "TimerStats.hpp"

TimerStats::TimerStats() {
    prevTime = millis();
}

void TimerStats::startTimer() {
    iters++;
    printIters++;

    if (iters == 500) {
        estimatedLoopTime = static_cast<double>(millis() - prevTime) / 500.0;
        loopsPerSecond = static_cast<long>(1000.0 / estimatedLoopTime);
        iters = 0;
        prevTime = millis();
    }
}

void TimerStats::printTimerData() const {
    Serial.print("Loops per second: ");
    Serial.print(loopsPerSecond);
    Serial.print(" | ");
    Serial.print("Estimated loop-time: ");
    Serial.print(estimatedLoopTime);
    Serial.println(" ms.");
}

TimerData TimerStats::getTimerData() {
    return {loopsPerSecond, estimatedLoopTime};
}