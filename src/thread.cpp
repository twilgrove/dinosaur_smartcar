#include "main.h"
#include <mutex>
#include <chrono>
#include <iomanip>  // 需要包含这个头文件
/* ------------------------------------------线程------------------------------------------ */
void opencv_thread()
{
    while (running)
    {
        CameraImgGetThread(Camera, Img_Store_p);
    }
}
/*
    负压风扇线程
*/
void fans_pwm_thread()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}



/*
    右轮控制线程
    @注意
        该函数包含读取编码器,死区重置,限幅
*/
void right_pid_pwm_thread()
{
    while (running)
    {
        r_now = static_cast<float>(std::abs(right_encoder.pulse_counter_update()));
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
        rp_duty = MAX_OUTPUT_LIMIT((int)rp_duty, WHEEL_MAX_PWM);
        rp_duty = MIN_OUTPUT_LIMIT((int)rp_duty, WHEEL_MIN_PWM);
        rp.set_duty(rp_duty);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
/*
    左轮控制线程
    @注意
        该函数包含读取编码器,死区重置,限幅
*/
void left_pid_pwm_thread()
{
    while (running)
    {
        l_now = left_encoder.pulse_counter_update();
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
        lp_duty = MAX_OUTPUT_LIMIT((int)lp_duty, WHEEL_MAX_PWM);
        lp_duty = MIN_OUTPUT_LIMIT((int)lp_duty, WHEEL_MIN_PWM);
        lp.set_duty(lp_duty);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
/*
    左轮控制线程
    @注意
        该函数包含读取舵机输出计算,限幅
*/
unsigned int servo_num = 0;
float public_para = 1;
void servo_pid_pwm_thread()
{
    const std::chrono::milliseconds initialization_time(40); // 初始化时间40ms
    auto program_start = std::chrono::steady_clock::now();    // 记录程序启动时间
    sp_duty = MIDO_sp;

    static float last_servo_turn = 0;  // 上一帧 servo_turn
    const float alpha = 0.9999f;          // 低通滤波系数
    const float max_delta = 150000.0f;  // 最大变化限幅值（根据速度调节）

    while (running)
    {
        servo_num++;
        auto now = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - program_start);

        if (elapsed_time < initialization_time) {
            sp_duty = MIDO_sp; // 强制中值
        } 
        else {
            // 原始误差计算（视觉计算出的方向偏差）
            double raw_servo_turn = (Data_Path_p->ServoDir * Data_Path_p->ServoAngle * 2000000 / 1000) * 2;

            // === 一阶低通滤波 ===
            // double filtered_servo_turn = alpha * raw_servo_turn + ((double)1 - alpha) * last_servo_turn;

            // === 限幅器限制突变 ===
            // float delta = raw_servo_turn - last_servo_turn;
            // if (fabs(delta) > max_delta) {
            //     raw_servo_turn = last_servo_turn + (delta > 0 ? max_delta : -max_delta);
            // }

            // 保存本次值作为下一次使用
            servo_turn = raw_servo_turn;
            last_servo_turn = servo_turn;
            if(JSON_trackConfigData.Forward < 80)
                public_para = 1 + (80-JSON_trackConfigData.Forward)/20;
            else public_para = 1;
            #if SPEED_MODE != 10
            sp_pid.set_kd(0.1);
            if (fabs(servo_turn) < 10000)
                sp_pid.set_kp(0.05);
            else if (fabs(servo_turn) < 40000)
                sp_pid.set_kp(0.12);
            else if (fabs(servo_turn) < 60000)
                sp_pid.set_kp(0.22);
            else if (fabs(servo_turn) < 100000)
                sp_pid.set_kp(0.32);
            else if (fabs(servo_turn) < 120000)
                sp_pid.set_kp(0.35);
            else if (fabs(servo_turn) < 140000)
                sp_pid.set_kp(0.45);
            else if (fabs(servo_turn) < 160000)
                sp_pid.set_kp(0.5);
            else if (fabs(servo_turn) < 180000)
                sp_pid.set_kp(0.58);
            else if (fabs(servo_turn) < 220000)
                sp_pid.set_kp(0.6);
            else
                sp_pid.set_kp(0.62);
            #endif

            #if SPEED_MODE == 10
            sp_pid.set_kd(0.05);
            if (fabs(servo_turn) < 10000)
                sp_pid.set_kp(0.03);
            else if (fabs(servo_turn) < 40000)
                sp_pid.set_kp(0.12);
            else if (fabs(servo_turn) < 60000)
                sp_pid.set_kp(0.30f);
            else if (fabs(servo_turn) < 100000)
                sp_pid.set_kp(0.35f);
            else if (fabs(servo_turn) < 140000)
                sp_pid.set_kp(0.40f);
            else if (fabs(servo_turn) < 180000)
                sp_pid.set_kp(0.5f*public_para*1.2);
            else
                sp_pid.set_kp(0.6f*public_para);
            #endif

            sp_duty = MIDO_sp - sp_pid.get(0, servo_turn);
            sp_duty = MAX_OUTPUT_LIMIT(sp_duty, SERVO_MAX_PWM);
            sp_duty = MIN_OUTPUT_LIMIT(sp_duty, SERVO_MIN_PWM);
        }

        // 差速补偿：通过舵角换算 K_Turn_
        angle = ((int32_t)sp_duty - MIDO_sp) / 5000;
        Rad = DEGTORAD(angle);
        K_Turn_ = FastTan(Rad) * 160 / 2 / 200;
        K_Turn_ = MAX_OUTPUT_LIMIT(K_Turn_, 5);
        K_Turn_ = MIN_OUTPUT_LIMIT(K_Turn_, -5);

        // 左右轮目标速度
        l_target = base_speed * (1 - K_Turn_);
        r_target = base_speed * (1 + K_Turn_);

        // PWM 限制并输出
        sp_duty = MAX_OUTPUT_LIMIT(sp_duty, SERVO_MAX_PWM);
        sp_duty = MIN_OUTPUT_LIMIT(sp_duty, SERVO_MIN_PWM);
        sp.set_duty(sp_duty);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void imu_thread()
{
    while (running)
    {
        // imu660ra_get_acc();
        // imu660ra_get_gyro();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void gpio_thread()
{
    while (running)
    {
        reset(key1.readValue());

        if (key2.readValue())
        {
            std::cout << "key2" << std::endl;
            sp_duty += 1000;
        }
        if (key3.readValue())
        {
            std::cout << "key3" << std::endl;
            sp_duty -= 1000;
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

        std::cout << std::left  // 左对齐
          << "angle:" << std::setw(10) << angle
          << "Rad:" << std::setw(10) << Rad
          << "Turn:" << std::setw(10) << K_Turn_
          << "l_targrt:" << std::setw(10) << l_target
          << "r_targrt:" << std::setw(10) << r_target
          << "error:" << std::setw(10) << servo_turn
          << "kind:" << std::setw(10) << Data_Path_p->Track_Kind
          << "l_now:" << std::setw(10) << std::fixed << std::setprecision(2) << l_now
          << "r_now:" << std::setw(10) << std::fixed << std::setprecision(2) << r_now
          << "sp_duty:" << std::setw(10) << (int32_t)sp_duty - MIDO_sp
          << "servo_num:" << std::setw(10) << servo_num << std::endl
          << "image_num:" << std::setw(10) << image__num 
          << "NumSearch:" << std::setw(10) << NumSearch 
          << "Forward:" << std::setw(10) << JSON_trackConfigData.Forward << std::endl;
          servo_num = 0;
          image__num = 0;
        // std::cout << "imu660ra_acc_x: " << imu660ra_acc_x << std::endl;
        // std::cout << "imu660ra_acc_y: " << imu660ra_acc_y << std::endl;
        // std::cout << "imu660ra_acc_z: " << imu660ra_acc_z << std::endl;
        // std::cout << "imu660ra_gyro_x: " << imu660ra_gyro_x << std::endl;
        // std::cout << "imu660ra_gyro_y: " << imu660ra_gyro_y << std::endl;
        // std::cout << "imu660ra_gyro_z: " << imu660ra_gyro_z << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
// std::mutex sender_video;  // 创建互斥锁
std::mutex img_mutex;

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

        //sender_video.lock();


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

        //sender_video.unlock();

        
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        // 互斥锁的作用域开始
        std::lock_guard<std::mutex> lock(image_mutex);  // 锁住图像数据
        
        if (!image_to_send.empty()) {
            g_udp_sender.sendImage(image_to_send);  // 发送图像
        }

        tty.printf("encoder: %f,%f\n", l_now, l_target);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}



enum TrackState {
    STABLE_STRAIGHT,
    STABLE_BEND,
    TRANSITION
};

TrackState current_state = STABLE_STRAIGHT;
const int CONFIRM_FRAMES = 10;     // 状态确认所需帧数
int transition_counter = 0;

TrackKind state_machine_filter(TrackKind raw_type) {
    switch (current_state) {
        case STABLE_STRAIGHT:
            if (raw_type == BEND_TRACK) {
                transition_counter++;
                if (transition_counter >= CONFIRM_FRAMES) {
                    current_state = STABLE_BEND;
                    transition_counter = 0;
                }
            } else {
                transition_counter = 0;
            }
            return STRIGHT_TRACK;

        case STABLE_BEND:
            if (raw_type == STRIGHT_TRACK) {
                transition_counter++;
                if (transition_counter >= CONFIRM_FRAMES) {
                    current_state = STABLE_STRAIGHT;
                    transition_counter = 0;
                }
            } else {
                transition_counter = 0;
            }
            return BEND_TRACK;

        case TRANSITION:
            // 处理过渡状态（可选）
            return raw_type;
    }
    return STRIGHT_TRACK;
}