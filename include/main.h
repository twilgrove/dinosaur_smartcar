#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <iostream>
#include <thread>
#include <atomic>
#include "pwm_ctrl.h"

void init();
void opencv_thread();
void fans_pwm_thread();
void right_pid_pwm_thread();
void left_pid_pwm_thread();
void servo_pid_pwm_thread();
void imu_thread();
void debug_thread();

#endif // __MAIN_H__