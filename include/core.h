#ifndef __MAIN_H__
#define __MAIN_H__

#include "head_system.h"

#include "path.h"

#include "data_process.h"
#include "image_process.h"
#include "data_config.h"

#include "pwm_ctrl.h"
#include "GPIO.h"
#include "key.h"
#include "pid.h"
#include "encoder.h"
#include "tty.h"

#define PROGRAM_NAME "Smart_Car"

#define WHEEL_MAX_PWM 20000
#define WHEEL_MIN_PWM 0
#define SPEED_DEADBAND 5 // 目标速度死区

#define SERVO_MAX_PWM 1600000
#define SERVO_MIN_PWM 1300000

#define MAX_OUTPUT_LIMIT(x, max) ((x) > (max) ? (max) : (x)) // 输出限幅
#define MIN_OUTPUT_LIMIT(x, min) ((x) < (min) ? (min) : (x)) // 输出限幅

void init();              // 初始化
void run();               // 准备运行
void project(int signum); // 程序状态输出
bool reset();             // 重置程序

bool apply_deadzone(float target_speed); // 在应用PID前进行目标速度死区补偿

void opencv_thread();        // opencv线程
void fans_pwm_thread();      // 负压风扇控制线程
void right_pid_pwm_thread(); // 右轮控制线程
void left_pid_pwm_thread();  // 左轮控制线程
void servo_pid_pwm_thread(); // 舵机控制线程
void imu_thread();           // 陀螺仪数据处理线程
void gpio_thread();          // GPIO控制线程
void debugi_thread();        // 调试输入线程
void debugo_thread();        // 调试输出线程
void car_thread();           // 小车控制线程
#endif                       // __MAIN_H__
