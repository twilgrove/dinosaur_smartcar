#include "core.h"

/* 运行标志 */
std::atomic<bool> running(true);

// /* 电机：频率20-50khz，周期20000-50000ns*/
// GPIO r_pin(61, "out", 1);
// pwm_ctrl rp(0, 0, 20000, 0, "right_motor");
// pid rp_pid(pid::Mode::INCREMENT, 0.1, 0.01, 0.001, 100, 100);
// GPIO l_pin(62, "out", 1);
// pwm_ctrl lp(1, 0, 20000, 0, "left_motor");
// pid lp_pid(pid::Mode::INCREMENT, 0.1, 0.01, 0.001, 100, 100);

// /* 舵机：频率50hz，周期20000ns，占空比500-2500ns*/
// pwm_ctrl sp(2, 0, 20000, 1500, "servo");
// pid sp_pid(pid::Mode::POSITION, 0.1, 0.01, 0.001, 100, 100);

// /* 按键 */
// Key key1(62, Key::up);

/* 编码器 */
ENCODER encoder(1, 62);
double encoder_value = 0;
int key_1_value = 0;

int main()
{
    try
    {
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
        gpio.join();
        debug.join();
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
    signal(SIGINT, project); // 设置进程终止处理函数

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
        encoder_value = encoder.pulse_counter_update();
    }
}

void left_pid_pwm_thread()
// key_1_value += key1.readValue() ? 1 : 0;
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

        std::cout << encoder_value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void gpio_thread()
{
    while (running)
    {
        // key_1_value += key1.readValue() ? 1 : 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
