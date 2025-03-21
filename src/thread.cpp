#include "main.h"

/* ------------------------------------------线程------------------------------------------ */
void opencv_thread()
{
    while (running)
    {
        CameraImgGetThread(Camera, Img_Store_p);
    }
}

void fans_pwm_thread()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

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
        rp_duty = MAX_OUTPUT_LIMIT(rp_duty, WHEEL_MAX_PWM);
        rp_duty = MIN_OUTPUT_LIMIT(rp_duty, WHEEL_MIN_PWM);
        rp.set_duty(rp_duty);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

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
        lp_duty = MAX_OUTPUT_LIMIT(lp_duty, WHEEL_MAX_PWM);
        lp_duty = MIN_OUTPUT_LIMIT(lp_duty, WHEEL_MIN_PWM);
        lp.set_duty(lp_duty);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void servo_pid_pwm_thread()
{
    while (running)
    {
        sp_duty = 1500000 + (Data_Path_p->ServoDir * Data_Path_p->ServoAngle * 2000000 / 1000);
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

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void debugo_thread()
{
    while (running)
    {

        std::cout << "l_now: " << l_now << std::endl;
        std::cout << "r_now: " << r_now << std::endl;
        std::cout << "sp_duty: " << sp_duty << std::endl;
        std::cout << "VmRSS: " << memory_monitor.getVmRSS() << "MB" << std::endl;

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

        // if (sender_ready)
        // {
        //     cv::line(Img_Store_p->Img_Track, cv::Point(0, 80), cv::Point(319, 80), cv::Scalar(0, 0, 255), 2);
        //     sender.sendFrame_pro(Img_Store_p->Img_Track);
        //     sender_ready = false;
        // }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
