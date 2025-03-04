#include "core.h"

/* 运行标志 */
std::atomic<bool> running(true);

/* 电机：频率20-50khz，周期20000-50000ns*/
GPIO r_pin(73, "out", 0);
pwm_ctrl rp(1, 0, 20000, 0, "right_motor");
pid rp_pid(pid::Mode::INCREMENT, 0.1, 0.01, 0.001, 100, 100);
GPIO l_pin(72, "out", 0);
pwm_ctrl lp(2, 0, 20000, 5000, "left_motor");
pid lp_pid(pid::Mode::INCREMENT, 0.1, 0.01, 0.001, 100, 100);

/* 舵机：频率50hz，周期20,000,000ns，占空比500,000-2,500,000ns*/
pwm_ctrl sp(8, 6, 20000000, 1500000, "servo");

// pid sp_pid(pid::Mode::POSITION, 0.1, 0.01, 0.001, 100, 100);

/* 按键 */
Key key1(16, Key::up);
Key key2(15, Key::up);
Key key3(14, Key::up);
Key key4(13, Key::up);

int key_2_value = 0;
int key_3_value = 0;
int key_4_value = 0;

/* 开关，低有效 */
GPIO switch1(20, "in");
GPIO switch2(21, "in");

bool switch1_value = 1;
bool switch2_value = 1;

/* 蜂鸣器，高有效 */
GPIO buzzer(12, "out", 0);

/* 编码器,10ms更新一次 */
// ENCODER left_encoder(0, 51);
// double left_encoder_value = 0;
// ENCODER right_encoder(3, 50);
// double right_encoder_value = 0;

int main()
{
    try
    {
        init();
        // run();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void fans_pwm_thread()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void right_pid_pwm_thread()
{
    while (running)
    {
        // right_encoder_value = right_encoder.pulse_counter_update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void left_pid_pwm_thread()
{
    while (running)
    {
        // left_encoder_value = left_encoder.pulse_counter_update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void servo_pid_pwm_thread()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void imu_thread()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void debug_thread()
{
    while (running)
    {
        // std::cout << "left_encoder_value: " << left_encoder_value << std::endl;
        // std::cout << "right_encoder_value: " << right_encoder_value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void gpio_thread()
{
    while (running)
    {
        if (key1.readValue())
            if (!reset())
                std::cout << "Program reset failed!!!" << std::endl;
        if (key2.readValue())
        {
            l_pin.setValue(1);
            r_pin.setValue(1);
        }
        if (key3.readValue())
        {
            l_pin.setValue(0);
            r_pin.setValue(0);
        }
        switch1_value = switch1.readValue();
        if (switch1_value == 0)
            project(-1);
        switch2_value = switch2.readValue();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void close()
{
    running = false;
}

void run()
{
    while (1)
    {
        if (key1.readValue())
            break;
        else
            usleep(10000);
    }
}

bool reset()
{
    std::cout << "Program reset..." << std::endl;

    // 获取程序的执行路径
    const char *program = "/proc/self/exe";
    char buffer[1024];

    ssize_t len = readlink(program, buffer, sizeof(buffer) - 1);
    if (len == -1)
    {
        std::cerr << "Error getting the program path: " << strerror(errno) << std::endl;
        return false; // 如果发生错误，返回 false
    }
    buffer[len] = '\0'; // Null-terminate the string

    // 使用 exec() 函数重新启动程序
    std::cout << "Restarting the program..." << std::endl;

    char *const args[] = {nullptr}; // 删除未使用的 env

    if (execv(buffer, args) == -1)
    {
        std::cerr << "Error restarting the program: " << strerror(errno) << std::endl;
        return false; // 如果 execv 失败，返回 false
    }

    return true;
}
