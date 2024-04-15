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

    int updateFreq = 5000;

    if (iters % updateFreq == 0) {
        estimatedLoopTime = static_cast<double>(millis() - prevTime) / static_cast<double>(updateFreq);
        loopsPerSecond = static_cast<long>(1000.0 / estimatedLoopTime);
        iters = 0;
        prevTime = millis();

        printTimerData();
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