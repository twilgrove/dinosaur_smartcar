#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <csignal>
#include "pwm_ctrl.h"

void init();
void opencv_thread();
void fans_pwm_thread();
void right_pid_pwm_thread();
void left_pid_pwm_thread();
void servo_pid_pwm_thread();
void imu_thread();
void debug_thread();

class project
{
public:
    inline project(std::string project_name)
        : project_name(std::move(project_name))
    {
        std::cout << "\33[33mPROGRAM:\33[0m" << project_name << " open success <^_^>" << std::endl;
        running = true;
    }

    static inline void signal_handler(int signum)
    {
        (void)signum;
        std::cout << std::endl
                  << "\33[33mPROGRAM:\33[0mreceived \33[35mSIGINT\33[0m, stopping the program..." << std::endl;
        running = false;
    }

    inline void end()
    {
        std::cout << "\33[33mPROGRAM:\33[0m" << "\33[36m" << project_name << "\33[0m" << " stop success (^_^)" << std::endl;
    }

    static std::atomic<bool> running;

private:
    std::string project_name;
};

#endif // __MAIN_H__