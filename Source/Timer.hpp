#pragma once

#include <cstdint>
#include <chrono>

#define timePoint std::chrono::steady_clock::time_point

struct Timer {
    inline static const long NS_PER_SECOND = 1000000000L;
    inline static const long MAX_NS_PER_UPDATE = 1000000000L;
    inline static const int  MAX_TICKS_PER_UPDATE = 100;
    float ticksPerSecond;
    timePoint lastTime;
    int ticks;
    float a;
    float timeScale = 1.0f;
    float fps = 0.0f;
    float passedTime = 0.0f;

    Timer(float ticksPerSecond) {
        this->ticksPerSecond = ticksPerSecond;
        this->lastTime = std::chrono::steady_clock::now();
    }

    void advanceTime() {
        timePoint now = std::chrono::steady_clock::now();
        long long passedNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTime).count();
        this->lastTime = now;
        if (passedNs < 0L)          passedNs = 0L;
        if (passedNs > MAX_NS_PER_UPDATE) passedNs = MAX_NS_PER_UPDATE;
        this->fps = 1000000000.0 / passedNs;
        this->passedTime += (float)passedNs * this->timeScale * this->ticksPerSecond / 1.0E9f;
        this->ticks = (int)this->passedTime;
        if (this->ticks > MAX_TICKS_PER_UPDATE) this->ticks = MAX_TICKS_PER_UPDATE;
        this->passedTime -= (float)this->ticks;
        this->a = this->passedTime;
    }
};

inline int64_t getTime() {
    // Source: https://stackoverflow.com/questions/2831841/how-to-get-the-time-in-milliseconds-in-c#2834294
    auto time = std::chrono::system_clock::now().time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time);
    return milliseconds.count();
}