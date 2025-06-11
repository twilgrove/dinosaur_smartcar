#include "image_deal.h"
#include "thread.h"
#include "isr.h"
#include "key_board.h"
#include "PID.h"
#include "my_control.h"
#include "traffic_circle.h"
#include "headfile.h"

void motor_servo_thread()
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

        r_now = static_cast<float>(std::abs(right_encoder.pulse_counter_update()));
        l_now = static_cast<float>(std::abs(left_encoder.pulse_counter_update()));
        if (apply_deadzone(r_target, WHEEL_SPEED_DEADBAND))
        {
            rp_duty = rp_pid.get(r_target, r_now);
        }
        else
        {
            rp_pid.reset();
            r_target = 0;
            rp_duty = 0;
        }
        if (apply_deadzone(l_target, WHEEL_SPEED_DEADBAND))
        {
            lp_duty = lp_pid.get(l_target, l_now);
        }
        else
        {
            lp_pid.reset();
            l_target = 0;
            lp_duty = 0;
        }

        rp_duty = MAX_OUTPUT_LIMIT(rp_duty, WHEEL_MAX_PLUS_ns);
        rp_duty = MIN_OUTPUT_LIMIT(rp_duty, WHEEL_MIN_PLUS_ns);
        lp_duty = MAX_OUTPUT_LIMIT(lp_duty, WHEEL_MAX_PLUS_ns);
        lp_duty = MIN_OUTPUT_LIMIT(lp_duty, WHEEL_MIN_PLUS_ns);
        // sp_duty = MAX_OUTPUT_LIMIT(sp_duty, SERVO_MAX_PLUS_ns);
        // sp_duty = MIN_OUTPUT_LIMIT(sp_duty, SERVO_MIN_PLUS_ns);

        sp.set_duty(sp_duty);
        lp.set_duty(lp_duty);
        rp.set_duty(rp_duty);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void debugi_thread()
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

void tiaoshi_thread()
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

void gpio_thread()
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