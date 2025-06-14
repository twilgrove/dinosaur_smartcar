#include "headfile.h"

int main()
{
    try
    {
        init();

        if (car.program_running)
        {
            std::thread opencv(opencv_thread);        // opencv线程  包含无畸变处理
            std::thread car(car_main_control_thread); // 小车控制线程
            std::thread hardware(hardware_control);   // 硬件控制线程
            std::thread debug1(debug1_thread);        // 高速调试线程
            std::thread debug2(debug2_thread);        // 低速调试线程
            std::thread IO(IO_thread);                // IO线程

            IO.join();
            car.join();
            opencv.join();
            debug1.join();
            hardware.join();
            debug2.join();
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

    gettimeofday(&car.start, NULL);
    ips200_init("/dev/fb0");
    ips200_show_string(10, 10, "Program is running...");

    car.IMU_running = imu_get_dev_info();
    car.Camera_running = CameraInit(Camera, 2, 100);

    IMU_Camera_Check();

#if IMG_SEND_EN
    std::cout << "ImgSender Init result: " << (ImgSender.init(DST_IP, UDP_PORT) ? "success" : "failed!!!") << std::endl;
#endif
#if KEY1_START_EN
    ips200_show_string(10, 30, "Wait key-1 to start...");
    std::cout << "Wait key-1 to start..." << std::endl;
    while (car.program_running)
    {
        if (key1.readValue())
        {
            ips200_show_string(10, 30, "All threads running...");
            std::cout << "All threads running..." << std::endl;
            break;
        }
    }
#endif
}
