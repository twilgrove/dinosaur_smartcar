#include "thread.h"

std::mutex img_mutex;
std::mutex image_mutex; // 定义一个互斥锁
cv::Mat image_to_send;  // 需要在多个线程之间共享的图像数据
UdpSender g_udp_sender;

void motor_servo_thread()
{
    while (running)
    {
        r_now = static_cast<float>(std::abs(right_encoder.pulse_counter_update()));
        l_now = static_cast<float>(std::abs(left_encoder.pulse_counter_update()));
        if (apply_deadzone(r_target, SPEED_DEADBAND))
        {
            rp_duty = rp_pid.get(r_target, r_now);
        }
        else
        {
            rp_pid.reset();
            r_target = 0;
            rp_duty = 0;
        }
        if (apply_deadzone(l_target, SPEED_DEADBAND))
        {
            lp_duty = lp_pid.get(l_target, l_now);
        }
        else
        {
            lp_pid.reset();
            l_target = 0;
            lp_duty = 0;
        }
        rp_duty = MAX_OUTPUT_LIMIT(rp_duty, WHEEL_MAX_PWM);
        rp_duty = MIN_OUTPUT_LIMIT(rp_duty, WHEEL_MIN_PWM);
        lp_duty = MAX_OUTPUT_LIMIT(lp_duty, WHEEL_MAX_PWM);
        lp_duty = MIN_OUTPUT_LIMIT(lp_duty, WHEEL_MIN_PWM);
        sp_duty = MAX_OUTPUT_LIMIT(sp_duty, SERVO_MAX_PWM);
        sp_duty = MIN_OUTPUT_LIMIT(sp_duty, SERVO_MIN_PWM);
        sp.set_duty(sp_duty);
        lp.set_duty(lp_duty);
        rp.set_duty(rp_duty);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

        // sender_video.lock();

        // if (sender_ready)
        // {
        //     //cv::Mat img_hello=Img_Store_p->Img_OTSU;
        //     //cv::line(Img_Store_p->Img_Color, cv::Point(0, 80), cv::Point(159, 80), cv::Scalar(0, 0, 255), 2);
        //     sender.sendFrame_pro(Img_Store_p->Img_Color);
        //     sender_ready = false;
        // }
        // if (sender_ready)
        // {
        //     sender.sendFrame_pro(Img_Store_p->Img_Track);
        //     sender_ready = false;
        // }

        // sender_video.unlock();

        // 互斥锁的作用域开始
        std::lock_guard<std::mutex> lock(image_mutex); // 锁住图像数据

        if (!image_to_send.empty())
        {
            g_udp_sender.sendImage(image_to_send); // 发送图像
        }

        // tty.printf("encoder: %f,%f\n", l_now, l_target);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}