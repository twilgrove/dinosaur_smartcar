#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <csignal>
#include "pwm_ctrl.h"
#include "GPIO.h"
#include "key.h"
#include "pid.h"
#include "encoder.h"
#define PROGRAM_NAME "Smart_Car"

void init();              // 初始化
void project(int signum); // 程序状态输出

void opencv_thread();        // opencv线程
void fans_pwm_thread();      // 负压风扇控制线程
void right_pid_pwm_thread(); // 右轮控制线程
void left_pid_pwm_thread();  // 左轮控制线程
void servo_pid_pwm_thread(); // 舵机控制线程
void imu_thread();           // 陀螺仪数据处理线程
void gpio_thread();          // GPIO控制线程
void debug_thread();         // 调试线程

extern std::atomic<bool> running; // 运行标志

#endif // __MAIN_H__
