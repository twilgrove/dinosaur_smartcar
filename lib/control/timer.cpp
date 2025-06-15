/**
 * @file timer.cpp
 * @author T.Grove
 * @brief
 * @date 2025-06-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "timer.h"

Timer::Timer(uint32_t period_ms, uint32_t tim_ms, uint32_t max_cycles)
    : count(0),
      threshold(tim_ms / period_ms),
      cycles_done(0),
      max_cycles(max_cycles),
      running(true)
{
}

bool Timer::tick()
{
    if (!running)
    {
        return false; // 停止状态，直接返回false
    }

    if (++count >= threshold)
    {
        count = 0;
        cycles_done++;

        if (max_cycles > 0 && cycles_done >= max_cycles)
        {
            running = false; // 达到最大循环次数，暂停运行
        }
        return true;
    }

    return false;
}

void Timer::reset()
{
    count = 0;
    cycles_done = 0;
    running = true;
}

void Timer::start()
{
    running = true;
}

void Timer::stop()
{
    running = false;
}

bool Timer::isRunning() const
{
    return running;
}
