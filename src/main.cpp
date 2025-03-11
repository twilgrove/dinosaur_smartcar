#include "core.h"

/* ----------------------------------------主函数---------------------------------------- */
int main()
{
    try // 异常处理
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
        std::thread car(car_main_control_thread);      // 小车控制线程

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
        car.join();
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

    std::cout << "\33[33m" << PROGRAM_NAME << ":\33[0m init complete " << std::endl;
}

void car_main_control_thread()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
