#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <csignal>
#include <unistd.h>
#include <cstring>
#include "pwm_ctrl.h"
#include "GPIO.h"
#include "key.h"
#include "pid.h"
#include "encoder.h"
#define PROGRAM_NAME "Smart_Car"
#define MAX_OUTPUT_LIMIT(x, max) ((x) > (max) ? (max) : (x)) // 输出限幅
#define MIN_OUTPUT_LIMIT(x, min) ((x) < (min) ? (min) : (x)) // 输出限幅

void init();              // 初始化
void run();               // 准备运行
void project(int signum); // 程序状态输出
bool reset();             // 重置程序

void opencv_thread();        // opencv线程
void fans_pwm_thread();      // 负压风扇控制线程
void right_pid_pwm_thread(); // 右轮控制线程
void left_pid_pwm_thread();  // 左轮控制线程
void servo_pid_pwm_thread(); // 舵机控制线程
void imu_thread();           // 陀螺仪数据处理线程
void gpio_thread();          // GPIO控制线程
void debug_thread();         // 调试线程

#endif // __MAIN_H__
