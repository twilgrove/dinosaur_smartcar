#include "core.h"

/* ----------------------------------------全局变量---------------------------------------- */

/* 运行标志 */
std::atomic<bool> running(true);

/* 串口 */
SerialPort tty("/dev/ttyS1", B9600);
uint8_t deta[8];
float value = 0;

/* 编码器,10ms更新一次 */
ENCODER left_encoder(0, 51);
double left_encoder_value = 0;
ENCODER right_encoder(3, 50);
double right_encoder_value = 0;

/* 左电机：频率20-50khz，周期20000-50000ns，占空比0-20000ns*/
uint32_t lp_duty = 0;
uint32_t lp_target = 0;
GPIO l_pin(72, "out", 0);
pwm_ctrl lp(2, 0, 20000, lp_duty, "left_motor");
pid lp_pid(pid::Mode::INCREMENT, 0.1, 0.01, 0.001, 100, 100);

/* 右电机：频率20-50khz，周期20000-50000ns，占空比0-20000ns*/
uint32_t rp_duty = 0;
uint32_t rp_target = 0;
GPIO r_pin(73, "out", 0);
pwm_ctrl rp(1, 0, 20000, rp_duty, "right_motor");
pid rp_pid(pid::Mode::INCREMENT, 0.1, 0.01, 0.001, 100, 100);

/* 舵机：频率50hz，周期20,000,000ns，占空比1300,000-1,600,000ns*/
uint32_t sp_duty = 1500000;
uint32_t sp_target = 0;
pwm_ctrl sp(8, 6, 20000000, sp_duty, "servo");
pid sp_pid(pid::Mode::POSITION, 0.1, 0.01, 0.001, 100, 100);

/* 按键 */
Key key1(16, Key::up);
Key key2(15, Key::up);
Key key3(14, Key::up);
Key key4(13, Key::up);

uint8_t key_1_value = 0;
uint8_t key_2_value = 0;
uint8_t key_3_value = 0;
uint8_t key_4_value = 0;

/* 开关，低有效 */
GPIO switch1(20, "in");
GPIO switch2(21, "in");

bool switch1_value = 1;
bool switch2_value = 1;

/* 蜂鸣器，高有效 */
GPIO buzzer(12, "out", 0);

/* ----------------------------------------主函数---------------------------------------- */
int main()
{
    try
    {
        init();
        run();
        /* 创建线程 */
        std::thread opencv(opencv_thread);             // opencv线程
        std::thread right_motor(right_pid_pwm_thread); // 右轮控制线程
        std::thread left_motor(left_pid_pwm_thread);   // 左轮控制线程
        std::thread servo(servo_pid_pwm_thread);       // 舵机控制线程
        std::thread fans(fans_pwm_thread);             // 负压风扇控制线程
        std::thread imu(imu_thread);                   // 陀螺仪数据处理线程
        std::thread gpio(gpio_thread);                 // GPIO控制线程
        std::thread debugi(debugi_thread);             // 调试输入线程
        std::thread debugo(debugo_thread);             // 调试输出线程

        /* 等待线程结束 */
        opencv.join();
        right_motor.join();
        left_motor.join();
        servo.join();
        fans.join();
        imu.join();
        gpio.join();
        debugi.join();
        debugo.join();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        project(-1);
    }
    return 0;
}

/* ----------------------------------------初始化---------------------------------------- */
void init()
{
    signal(SIGINT, project); // 设置进程终止处理函数

    std::cout << "\33[33m" << PROGRAM_NAME << ":\33[0m init complete " << std::endl;
}

/* ----------------------------------------线程---------------------------------------- */
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
        right_encoder_value = -right_encoder.pulse_counter_update();
        rp_duty = MAX_OUTPUT_LIMIT(rp_duty, 20000);
        rp_duty = MIN_OUTPUT_LIMIT(rp_duty, 0);
        rp.set_duty(rp_duty);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void left_pid_pwm_thread()
{
    while (running)
    {
        left_encoder_value = left_encoder.pulse_counter_update();
        lp_duty = MAX_OUTPUT_LIMIT(lp_duty, 20000);
        lp_duty = MIN_OUTPUT_LIMIT(lp_duty, 0);
        lp.set_duty(lp_duty);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void servo_pid_pwm_thread()
{
    while (running)
    {
        sp_duty = MAX_OUTPUT_LIMIT(sp_duty, 1600000);
        sp_duty = MIN_OUTPUT_LIMIT(sp_duty, 1300000);
        sp.set_duty(sp_duty);
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

void gpio_thread()
{
    while (running)
    {
        if (key1.readValue())
        {
            std::cout << "key1" << std::endl;
            reset();
        }
        if (key2.readValue())
        {
            std::cout << "key2" << std::endl;
        }
        if (key3.readValue())
        {
            std::cout << "key3" << std::endl;
        }
        if (key4.readValue())
        {
            std::cout << "key4" << std::endl;
        }

        switch1_value = switch1.readValue();
        switch2_value = switch2.readValue();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void debugo_thread()
{
    while (running)
    {
        // 发送调试信息
        tty.printf("encoder: %f\n", left_encoder_value);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void debugi_thread()
{
    while (running)
    {
        if (tty.readData(deta, 8))
        {
            if (deta[0] == 0x55)
            {
                std::memcpy(&value, &deta[4], sizeof(float)); // 复制4字节到 float 变量
                if (deta[1] == 0x20)
                {
                    if (deta[2] == 0x01) // 通道1
                    {
                        std::cout << "channel_1" << std::endl;
                    }
                    else if (deta[2] == 0x02) // 通道2
                    {
                        std::cout << "channel_2" << std::endl;
                    }
                    else if (deta[2] == 0x03) // 通道3
                    {
                        std::cout << "channel_3" << std::endl;
                    }
                    else if (deta[2] == 0x04) // 通道4
                    {
                        std::cout << "channel_4" << std::endl;
                    }
                    else if (deta[2] == 0x05) // 通道5
                    {
                        std::cout << "channel_5" << std::endl;
                    }
                    else if (deta[2] == 0x06) // 通道6
                    {
                        std::cout << "channel_6" << std::endl;
                    }
                    else if (deta[2] == 0x07) // 通道7
                    {
                        std::cout << "channel_7" << std::endl;
                    }
                    else if (deta[2] == 0x08) // 通道8
                    {
                        std::cout << "channel_8" << std::endl;
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
