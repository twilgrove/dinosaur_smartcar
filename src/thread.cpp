#include "headfile.h"

void car_main_control_thread()
{
    // /*john code began*/
    // Tread_Init();
    // /*john code end*/
    while (car.program_running)
    {
        // /*john code began*/
        // Choose_Kind(&Control_john); // 获取赛道类型
        // Get_Sp_Duty(&Control_john); // 获取舵机占空比
        // /*john code end*/
        // sp_duty = sp_duty * 0.7 + last_sp_duty * 0.3;
        // last_sp_duty = sp_duty;
        // /*john code began*/
        // Get_Turn(sp_duty);                // 获取后轮系数
        // Get_Speed(chujie, &Control_john); // 速度决策
        // /*john code end*/

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void hardware_control()
{
    static uint8_t t_10ms = 0;
    while (car.program_running)
    {
        // Update_motor();

        if (t_10ms++ >= 2)
        {
            // Update_servo();
            t_10ms = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void IO_thread()
{
    while (car.program_running)
    {

        if (key1.readValue())
        {
            std::cout << "key-1..." << std::endl;
            ips200_clear();
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
    static uint8_t t_100ms = 0;
    ips200_show_string(10, 70, "Frame rate:");
    ips200_show_string(10, 50, "run time:");
    while (car.program_running)
    {

        if (t_100ms++ >= 5)
        {
            t_100ms = 0;
        }
#if VOFA_DEBUG_EN
        if (tty.readData(tty_data, 8))
        {
            if (tty_data[0] == 0x55)
            {
                std::memcpy(&tty_value, &tty_data[4], sizeof(float)); // 复制4字节到 float 变量
                tty_value_int = (uint32_t)tty_value;
                if (tty_data[1] == 0xcc)
                {
                    if (tty_data[2] == 0x01) // 通道1
                    {
                        std::cout << "channel_1" << std::endl;
                        lp_pid.config.kp = tty_value;
                    }
                    else if (tty_data[2] == 0x02) // 通道2
                    {
                        std::cout << "channel_2" << std::endl;
                        lp_pid.config.ki = tty_value;
                    }
                    else if (tty_data[2] == 0x03) // 通道3
                    {
                        std::cout << "channel_3" << std::endl;
                        lp_pid.config.kd = tty_value;
                    }
                    else if (tty_data[2] == 0x04) // 通道4
                    {
                        std::cout << "channel_4" << std::endl;
                        lp_pid.config.min_output = -tty_value;
                        lp_pid.config.max_output = tty_value;
                    }
                    else if (tty_data[2] == 0x05) // 通道5
                    {
                        std::cout << "channel_5" << std::endl;
                        l_target = tty_value;
                    }
                }
            }
        }
#endif
#if IMG_SEND_EN
        if (!image_to_send.empty())
        {
            image_mutex.lock();
            ImgSender.sendImage(image_to_send); // 发送图像
            image_mutex.unlock();
        }
#endif

        ips200_show_gray_image(0, 220, &image_show[0][0], 240, 90);

        Update_time();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void debug2_thread()
{
    while (car.program_running)
    {
        // /*john code began*/
        // std::cout << std::left
        //           << "opencv:" << std::setw(10) << opencv_v
        //           << "control:" << std::setw(10) << control_v
        //           << "sp_duty:" << std::setw(10) << (int32_t)sp_duty - SERVO_MID_PLUS_ns << std::endl;
        // opencv_v = 0;
        // control_v = 0;
        // /*john code end*/

        ips200_show_uint(100, 70, car.cam_frame, 3);
        car.cam_frame = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
