#include "timer.h"

Timer::~Timer()
{
	if (mRunning) { stop(); }
}

void Timer::start(const Interval& interval, const Timeout& timeout)
{
    mRunning = true;
    mThread = std::thread([this, interval, timeout]
        {
        while (mRunning)
        {
            std::this_thread::sleep_for(interval);
            timeout();
        }});
}

void Timer::stop()
{
    mRunning = false;
    mThread.join();
}

