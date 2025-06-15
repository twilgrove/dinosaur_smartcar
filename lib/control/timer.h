/**
 * @file timer.h
 * @author T.Grove
 * @brief
 * @date 2025-06-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef TIMER_H
#define TIMER_H

#include <cstdint>

class Timer
{
public:
    Timer(uint32_t period_ms, uint32_t tim_ms, uint32_t max_cycles = 0);
    // period_ms：tick间隔毫秒，tim_ms：时长，max_cycles：最大循环次数（0表示无限）

    bool tick();  // 返回是否到达阈值且计数未停止
    void reset(); // 重置计数器和状态
    void start(); // 重新开始运行（允许tick计数）
    void stop();  // 停止计数

    bool isRunning() const; // 查询是否在运行状态

private:
    uint32_t count;       // 当前tick计数
    uint32_t threshold;   // 达到阈值后触发
    uint32_t cycles_done; // 已完成的循环次数
    uint32_t max_cycles;  // 最大循环次数，0表示无限制
    bool running;         // 是否允许计数
};

#endif // TIMER_H
