#include "headfile.h"

int main()
{
    try
    {
        init();

        if (car.program_running)
        {
            std::thread opencv(opencv_thread);        // opencv线程  包含无畸变处理
            std::thread car(car_main_control_thread); // 算法控制线程
            std::thread hardware(hardware_control);   // 硬件控制线程
            std::thread h_debug(h_debug_thread);      // 高速调试线程
            std::thread l_debug(l_debug_thread);      // 低速调试线程
            std::thread IO(IO_thread);                // IO线程

            IO.join();
            car.join();
            opencv.join();
            h_debug.join();
            hardware.join();
            l_debug.join();
        }
        std::cout << "program exit..." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        project_manage(-1);
    }
    return 0;
}

void init()
{
    signal(SIGINT, project_manage); // 设置进程终止处理函数

    std::cout << "ImgSender Init  " << (ImgSender.init(DST_IP, UDP_PORT) ? "success" : "failed!!!") << std::endl;

    ips200_init("/dev/fb0");
    ips200_show_string(10, 10, "Program is starting...");

    car.IMU_running = imu_get_dev_info();
    car.Camera_running = CameraInit(Camera, 2, 100);

    IMU_Camera_Check();
#if KEY1_START_EN
    ips200_show_string(10, 30, "Wait key-1 to start...");
    std::cout << "Wait key-1 to start..." << std::endl;
    while (car.program_running)
    {
        if (key1.readValue())
        {
            std::cout << "All threads running..." << std::endl;
            ips200_show_string(10, 30, "All threads running...");
            break;
        }
    }
#endif

    Update_ips();
    l_target = 5;
    r_target = 5;
}

void h_debug_thread()
{
    static Timer t100ms(H_DEBUG_THREAD_PERIOD, 100);

    while (car.program_running)
    {

        if (t100ms.tick())
        {
            if (car.IMG_display)
                ips200_show_gray_image(0, 220, &image_show[0][0], 240, 90);

            if (!image_to_send.empty() && car.IMG_send)
            {
                image_mutex.lock();
                ImgSender.sendImage(image_to_send); // 发送图像
                image_mutex.unlock();
            }
        }

#if VOFA_DEBUG_EN

#define wheel rp_pid
#define target r_target
#define now r_now
#define duty rp_duty
#define pid_output rp_pid.last_output
        if (tty.readData(tty_data, 8))
        {
            if (tty_data[0] == 0x55)
            {
                std::memcpy(&tty_value, &tty_data[4], sizeof(float)); // 复制4字节到 float 变量
                if (tty_data[1] == 0xcc)
                {
                    if (tty_data[2] == 0x01) // 通道1
                    {
                        std::cout << "channel_1" << std::endl;
                        wheel.config.kp = tty_value;
                        std::cout << "Kp:" << wheel.config.kp << std::endl;
                    }
                    else if (tty_data[2] == 0x02) // 通道2
                    {
                        std::cout << "channel_2" << std::endl;
                        wheel.config.ki = tty_value;
                        std::cout << "Ki:" << wheel.config.ki << std::endl;
                    }
                    else if (tty_data[2] == 0x03) // 通道3
                    {
                        std::cout << "channel_3" << std::endl;
                        wheel.config.kd = tty_value;
                        std::cout << "Kd:" << wheel.config.kd << std::endl;
                    }
                    else if (tty_data[2] == 0x04) // 通道4
                    {
                        std::cout << "channel_4" << std::endl;
                        wheel.config.max_integral = tty_value;
                        wheel.config.min_integral = -tty_value;
                        std::cout << "max_integral:" << wheel.config.max_integral << std::endl;
                        std::cout << "min_integral:" << wheel.config.min_integral << std::endl;
                    }
                    else if (tty_data[2] == 0x05) // 通道5
                    {
                        std::cout << "channel_5" << std::endl;
                        target = tty_value;
                        std::cout << "l_target:" << target << std::endl;
                    }
                }
            }
        }
        tty.printf("data:%.2f,%.2f,%d\n", now, target, duty);
#endif

        std::this_thread::sleep_for(std::chrono::milliseconds(H_DEBUG_THREAD_PERIOD));
    }
}

void hardware_control()
{
    static Timer t10ms(HARDWARE_THREAD_PERIOD, 10);
    while (car.program_running)
    {
        Update_runing_state();
        Update_motor();

        if (t10ms.tick())
        {
            Update_servo();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(HARDWARE_THREAD_PERIOD));
    }
}