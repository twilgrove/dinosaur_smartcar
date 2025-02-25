#include "main.h"

/* 线程运行标志 */
std::atomic<bool> running(true);

/* 电机：频率20-50khz，周期20000-50000ns*/
pwm_ctrl rp(0, 0, 20000, 0, "right_motor");
pwm_ctrl lp(1, 0, 20000, 0, "left_motor");

/* 舵机：频率50hz，周期20000ns，占空比500-2500ns*/
pwm_ctrl sp(2, 0, 20000, 1500, "servo");

int main()
{
    /* 初始化 */
    init();

    /* 创建线程 */
    std::thread opencv(opencv_thread);             // opencv线程
    std::thread right_motor(right_pid_pwm_thread); // 右轮控制线程
    std::thread left_motor(left_pid_pwm_thread);   // 左轮控制线程
    std::thread servo(servo_pid_pwm_thread);       // 舵机控制线程
    std::thread fans(fans_pwm_thread);             // 负压风扇控制线程
    std::thread imu(imu_thread);                   // 陀螺仪数据处理线程
    std::thread debug(debug_thread);               // 调试线程

    /* 等待线程结束 */
    opencv.join();
    right_motor.join();
    left_motor.join();
    servo.join();
    fans.join();
    imu.join();
    debug.join();

    return 0;
}

void init()
{
    std::cout << "正在初始化..." << std::endl;
}

void opencv_thread()
{
    while (running)
    {
    }
}
void fans_pwm_thread()
{
    while (running)
    {
    }
}

void right_pid_pwm_thread()
{
    while (running)
    {
    }
}

void left_pid_pwm_thread()
{
    while (running)
    {
    }
}

void servo_pid_pwm_thread()
{
    while (running)
    {
    }
}

void imu_thread()
{
    while (running)
    {
    }
}

void debug_thread()
{
    while (running)
    {
    }
}