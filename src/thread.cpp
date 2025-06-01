#include "image_deal.h"
#include "thread.h"
#include "isr.h"
#include "key_board.h"
#include "PID.h"
#include "my_control.h"
#include "traffic_circle.h"
#include "headfile.h"
std::mutex img_mutex;
std::mutex image_mutex; // 定义一个互斥锁
cv::Mat image_to_send;  // 需要在多个线程之间共享的图像数据
UdpSender g_udp_sender;
int last_sp_duty=1522000;
void motor_servo_thread()
{   
    sp.set_duty(MIDO_sp);
    const std::chrono::milliseconds initialization_time(1000); // 初始化时间400ms
    auto program_start = std::chrono::steady_clock::now();    // 记录程序启动时间
    sp_duty = MIDO_sp;
    sp_pid.set_kp(1.05);
    while (running)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - program_start);
        if (elapsed_time < initialization_time) {
            sp_duty = MIDO_sp; // 强制中值
        } 
        else{
            double servo_turn = Get_Point - 87;
            // sp_pid.set_kd(0.01);
            if (fabs(servo_turn) < 2.5)
                sp_pid.set_kp(0.05);   
            else if (fabs(servo_turn) < 10)
                sp_pid.set_kp(0.2);
            else if (fabs(servo_turn) < 15)
                sp_pid.set_kp(0.8);
            // else if (fabs(servo_turn) < 25)
            //     sp_pid.set_kp(1);
            // else if (fabs(servo_turn) < 30)
            //     sp_pid.set_kp(1.08);
            // else if (fabs(servo_turn) < 35)
            //     sp_pid.set_kp(1.1);
            // else if (fabs(servo_turn) < 40)
            //     sp_pid.set_kp(1.13);
            // else if (fabs(servo_turn) < 45)
            //     sp_pid.set_kp(1.15);
            else if (fabs(servo_turn) < 50)
                sp_pid.set_kp(1.05);
            else
            sp_pid.set_kp(1.2);
            sp_duty=MIDO_sp+sp_pid.get(0,servo_turn*4000);
        }
        // sp_duty=sp_duty*0.4+last_sp_duty*0.6;
        // last_sp_duty=sp_duty;

        double angle = ((int32_t)sp_duty - MIDO_sp) / 5000;
        double Rad = DEGTORAD(angle);
        double K_Turn_ = FastTan(Rad) * 160 / 2 / 200;
        K_Turn_ = MAX_OUTPUT_LIMIT(K_Turn_, 5);
        K_Turn_ = MIN_OUTPUT_LIMIT(K_Turn_, -5);

        // 左右轮目标速度
        l_target = 8 * (1 - K_Turn_);
        r_target = 8 * (1 + K_Turn_);

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
        // sp_duty = MAX_OUTPUT_LIMIT(sp_duty, SERVO_MAX_PWM);
        // sp_duty = MIN_OUTPUT_LIMIT(sp_duty, SERVO_MIN_PWM);
        
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
        // std::lock_guard<std::mutex> lock(image_mutex); // 锁住图像数据

        // if (!image_to_send.empty())
        // {
        //     g_udp_sender.sendImage(image_to_send); // 发送图像
        // }

        // tty.printf("encoder: %f,%f\n", l_now, l_target);
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }
}