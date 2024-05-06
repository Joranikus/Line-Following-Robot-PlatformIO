
#ifndef LINEFOLLOWINGROBOT_TIMERSTATS_HPP
#define LINEFOLLOWINGROBOT_TIMERSTATS_HPP

struct TimerData {
    long loopsPerSecond;
    double estimatedLoopTime;
    double distancePerLoop;
};

class TimerStats {

public:
    TimerStats();

    void startTimer();
    void printTimerData() const;

    unsigned long long printIters = 0;
    int updateFreq = 1000;
    static constexpr double maxSpeed = 4.0; // Extremely optimistic top speed (m/s)

private:
    unsigned long prevTime;

    long loopsPerTimeunit = 0;
    double estimatedLoopTime = 0;
    double distancePerLoop = 0;
    unsigned long iters = 0;

    TimerData getTimerData();
};


#endif //LINEFOLLOWINGROBOT_TIMERSTATS_HPP
