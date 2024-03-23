//
// Created by Øystein Bringsli.
//

#ifndef LINEFOLLOWINGROBOT_TIMERSTATS_HPP
#define LINEFOLLOWINGROBOT_TIMERSTATS_HPP

struct TimerData {
    long loopsPerSecond;
    double estimatedLoopTime;
};

class TimerStats {

public:
    TimerStats();

    void startTimer();
    void printTimerData() const;

    unsigned long long printIters = 0;

private:
    unsigned long prevTime;

    long loopsPerSecond = 0;
    double estimatedLoopTime = 0;
    unsigned long iters = 0;

    TimerData getTimerData();
};


#endif //LINEFOLLOWINGROBOT_TIMERSTATS_HPP
