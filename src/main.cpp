#include "core.h"

/* 运行标志 */
std::atomic<bool> running(true);

/* 电机：频率20-50khz，周期20000-50000ns*/
// GPIO r_pin(61, "out", 1);
// pwm_ctrl rp(0, 0, 20000, 0, "right_motor");
// GPIO l_pin(62, "out", 1);
// pwm_ctrl lp(1, 0, 20000, 0, "left_motor");

/* 舵机：频率50hz，周期20000ns，占空比500-2500ns*/
// pwm_ctrl sp(2, 0, 20000, 1500, "servo");

/* 按键 */
Key key1(62, Key::up);

int key_1_value = 0;

int main()
{
    try
    {
        signal(SIGINT, project);
        init();

        /* 创建线程 */
        std::thread opencv(opencv_thread);             // opencv线程
        std::thread right_motor(right_pid_pwm_thread); // 右轮控制线程
        std::thread left_motor(left_pid_pwm_thread);   // 左轮控制线程
        std::thread servo(servo_pid_pwm_thread);       // 舵机控制线程
        std::thread fans(fans_pwm_thread);             // 负压风扇控制线程
        std::thread imu(imu_thread);                   // 陀螺仪数据处理线程
        std::thread gpio(gpio_thread);                 // GPIO控制线程
        std::thread debug(debug_thread);               // 调试线程

        /* 等待线程结束 */
        opencv.join();
        right_motor.join();
        left_motor.join();
        servo.join();
        fans.join();
        imu.join();
        debug.join();
        gpio.join();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        project(-1);
    }
    return 0;
}

void init()
{
    project(0);
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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "key_1_value: " << key_1_value << std::endl;
    }
}

void gpio_thread()
{
    while (running)
    {
        key_1_value += key1.readValue() ? 1 : 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void key_thread()
{
    while (running)
    {
    }
}