#pragma once
#include <thread>
#include <chrono>
#include <functional>
#include <cstdio>
#include <atomic>

class Timer {
public:
    ~Timer();
    
    typedef std::chrono::milliseconds Interval;
    typedef std::function<void(void)> Timeout;
    
    void start(const Interval& interval, const Timeout& timeout);
    void stop();
    

private:
    std::thread mThread{};
    std::atomic_bool mRunning{};
};
