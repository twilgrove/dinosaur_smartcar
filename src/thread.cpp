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
    double servo_turn;
    while (running)
    {
        sp_v++;
        auto now = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - program_start);
        //计算赛道误差
        std::vector<double> data(55);
        for (int i = 10; i < 65; ++i) {
            data[i - 10] = static_cast<double>(center[i]);
        }
        std::vector<double> filtered_data = first_order_filter(data, 0.2);//一阶低通滤波
        auto [rss_linear, slope] = calc_rss_linear(filtered_data); // 一阶
        double rss_quad = calc_rss_quadratic(filtered_data); //二阶
        std::cout << "Linear RSS: " << rss_linear << std::endl;
        std::cout << "slope: " << slope << std::endl; 
        std::cout << "Quadratic RSS: " << rss_quad << std::endl;
        std::cout << "wan_flag: " << (int)wan_flag << std::endl;
        //判断赛道类型
        bool track_kind;
        
        if(((fabs(slope) < 0.3 && rss_linear < 1000)&&wan_flag==0&&white_num_col_max > 62)||
            (wan_flag && fabs(slope) < 0.15 && rss_linear < 50 && white_num_col_max > 62)
        )
        {
            if(wan_flag)
                line_num++;
            if(line_num >= 8)
            {
                line_num = 0;
                wan_flag = 0;
            }
            track_kind = false;
            std::cout << "line" << std::endl;
        }
        // if ((fabs(slope) > 0.4 && rss_linear > 1000) //直到大偏转
        //     || (rss_quad < 0.02*rss_linear) //曲线
        //     || (rss_linear > 5000 &&  rss_quad < 3000)) // 尾部出现勾起 巨大误差 
        else{
            if(wan_flag == 0)
                wan_num++;
            if(wan_num >= 5){
                wan_flag = 1;
                wan_num = 0;
                }
            track_kind = true;
            std::cout << "quxian" << std::endl;
        }
            
        float para = ((70 - white_num_col_max)*1.2+fabs(white_num_col_line - 80)*0.8)/150;

        if (elapsed_time < initialization_time) {
            sp_duty = MIDO_sp; // 强制中值
        } 
        else{
            servo_turn = Point_Weight() - 80;
            if(track_kind == false)
            {
                if (fabs(servo_turn) < 5)
                    sp_pid.set_kp(0.2);  
                else
                    sp_pid.set_kp(0.4*(1+para));   
            }
            else
            {  
                // sp_pid.set_kd(0.01);
                if (fabs(servo_turn) < 1.5)
                    sp_pid.set_kp(0.2);   
                // else if (fabs(servo_turn) < 20)
                //     sp_pid.set_kp(0.3);
                // else if (fabs(servo_turn) < 30)
                //     sp_pid.set_kp(0.8);
                else
                    sp_pid.set_kp(0.52*(1+para) );
                
            }
            sp_duty=MIDO_sp+sp_pid.get(0,servo_turn*4000);
        }
        sp_duty=sp_duty*0.7+last_sp_duty*0.3;
        last_sp_duty=sp_duty;
        
        double angle = ((int32_t)sp_duty - MIDO_sp) / 5000;
        double Rad = DEGTORAD(angle);
        double K_Turn_ = FastTan(Rad) * 160 / 2 / 200;
        K_Turn_ = MAX_OUTPUT_LIMIT(K_Turn_, 5);
        K_Turn_ = MIN_OUTPUT_LIMIT(K_Turn_, -5);
        if(chujie || car_flag == 5)
        {
            l_target = 0;
            r_target = 0;
        }
        else if (elapsed_time < initialization_time){
            l_target = 4 ;
            r_target = 4 ;
        }// 左右轮目标速度
        else{
            if(track_kind) //弯道
            {
                buzzer.setValue(0);
                l_target = 11 * (1 - K_Turn_);
                r_target = 11 * (1 + K_Turn_);
            }
            else{
                //buzzer.setValue(1);
                l_target = 16;
                r_target = 16;
            }
            
        }
       
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
        

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
unsigned char image_transposed[188][70];

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

        // 互斥锁的作用域开始image_scaled
        // std::lock_guard<std::mutex> lock(image_mutex); // 锁住图像数据

        if (!image_to_send.empty())
        {
            g_udp_sender.sendImage(image_to_send); // 发送图像
            // transpose_matrix(&image_use[0][0], &image_transposed[0][0], 188, 70);
            // ips200_show_gray_image(0, 0, &image_use[0][0], 188, 70); 
        }
        
        // tty.printf("encoder: %f,%f\n", l_now, l_target);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void tiaoshi_thread()
{
    while(running)
    {
        
        std::cout << std::left  // 左对齐
          << "opencv:" << std::setw(10) << opencv_v 
          << "control:" << std::setw(10) << control_v 
          << "sp:" << std::setw(10) << sp_v 
          << "sp_duty:" << std::setw(10) << (int32_t)sp_duty - MIDO_sp << std::endl;
        opencv_v = 0;
        control_v = 0;
        sp_v = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void gpio_thread()
{
    while (running)
    {
        

        if (key1.readValue())
        {
            Camera.release();
            reset(1);
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
        tty.printf("qqq:%d,%d,%d,%.4f,%d\n",Points-70,0,Point_last1-70,(double)sp_duty/10000,MIDO_sp/10000);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}