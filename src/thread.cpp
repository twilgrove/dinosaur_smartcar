#include "headfile.h"

void car_main_control_thread()
{
    /*john code began*/
    Tread_Init();
    /*john code end*/
    while (running)
    {
        /*john code began*/
        Choose_Kind(&Control_john); // 获取赛道类型
        Get_Sp_Duty(&Control_john); // 获取舵机占空比
        /*john code end*/
        sp_duty = sp_duty * 0.7 + last_sp_duty * 0.3;
        last_sp_duty = sp_duty;
        /*john code began*/
        Get_Turn(sp_duty);                // 获取后轮系数
        Get_Speed(chujie, &Control_john); // 速度决策
        /*john code end*/

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void hardware_control()
{
    uint8_t t_10ms = 0;
    while (running)
    {
        update_motor();

        if (t_10ms++ >= 2)
        {
            update_servo();
            t_10ms = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void IO_thread()
{
    while (running)
    {

        if (key1.readValue())
        {
            std::cout << "key-1..." << std::endl;
            Camera.release();
            reset(1);
        }
        if (key2.readValue())
        {
            std::cout << "key-2..." << std::endl;
        }
        if (key3.readValue())
        {
            std::cout << "key-3..." << std::endl;
        }
        if (key4.readValue())
        {
            std::cout << "key-4..." << std::endl;
        }

        switch1_value = switch1.readValue();
        switch2_value = switch2.readValue();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void debug1_thread()
{
    while (running)
    {
        // if (tty.readData(deta, 8))
        // {
        //     if (deta[0] == 0x55)
        //     {
        //         std::memcpy(&value, &deta[4], sizeof(float)); // 复制4字节到 float 变量
        //         value_int = (uint32_t)value;
        //         if (deta[1] == 0xcc)
        //         {
        //             if (deta[2] == 0x01) // 通道1
        //             {
        //                 std::cout << "channel_1" << std::endl;
        //                 lp_pid.set_kp(value);
        //             }
        //             else if (deta[2] == 0x02) // 通道2
        //             {
        //                 std::cout << "channel_2" << std::endl;
        //                 lp_pid.set_ki(value);
        //             }
        //             else if (deta[2] == 0x03) // 通道3
        //             {
        //                 std::cout << "channel_3" << std::endl;
        //                 lp_pid.set_kd(value);
        //             }
        //             else if (deta[2] == 0x04) // 通道4
        //             {
        //                 std::cout << "channel_4" << std::endl;
        //                 lp_pid.set_delta_output(-value, value);
        //             }
        //             else if (deta[2] == 0x05) // 通道5
        //             {
        //                 std::cout << "channel_5" << std::endl;
        //                 l_target = value;
        //             }
        //         }
        //     }
        // }
        // // 发送调试信息
        // tty.printf("encoder: %f,%f\n", l_now, l_target);

        // transpose_matrix(&image_use[0][0], &image_transposed[0][0], 188, 70);
        // ips200_show_gray_image(0, 0, &image_use[0][0], 188, 70);
#if IMG_SEND
        if (!image_to_send.empty())
        {
            std::lock_guard<std::mutex> lock(image_mutex);
            ImgSender.sendImage(image_to_send); // 发送图像
            std::lock_guard<std::mutex> ulock(image_mutex);
        }
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void debug2_thread()
{
    while (running)
    {
        /*john code began*/
        std::cout << std::left
                  << "opencv:" << std::setw(10) << opencv_v
                  << "control:" << std::setw(10) << control_v
                  << "sp_duty:" << std::setw(10) << (int32_t)sp_duty - SERVO_MID_PLUS_ns << std::endl;
        opencv_v = 0;
        control_v = 0;
        /*john code end*/
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
