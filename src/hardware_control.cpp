#include "headfile.h"

void update_motor()
{
    r_now = static_cast<float>(std::abs(right_encoder.pulse_counter_update()));
    l_now = static_cast<float>(std::abs(left_encoder.pulse_counter_update()));
    if (rp_pid.apply_deadzone(r_target, WHEEL_SPEED_DEADBAND))
    {
        rp_duty = rp_pid.get(r_target, r_now);
    }
    else
    {
        rp_pid.reset();
        r_target = 0;
        rp_duty = 0;
    }

    if (lp_pid.apply_deadzone(l_target, WHEEL_SPEED_DEADBAND))
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

    lp.set_duty(lp_duty);
    rp.set_duty(rp_duty);
}

void update_servo()
{
    sp.set_duty(sp_duty);
}

void update_isp()
{
}

void IMU_Camera_Check()
{

    if (car.Camera_running && car.IMU_running)
    {
        ips200_show_string(10, 30, "Camera and IMU init OK!!!");
        return;
    }
    else
    {
        if (!car.Camera_running && !car.IMU_running)
            ips200_show_string(10, 30, "Camera and IMU are init failed!!!");
        else if (!car.Camera_running)
            ips200_show_string(10, 30, "Camera is init failed!!!");
        else if (!car.IMU_running)
            ips200_show_string(10, 30, "IMU is init failed!!!");

        ips200_show_string(10, 50, "Wait key-1 to restart...");
        std::cout << "Wait key-1 to restart..." << std::endl;

        while (!key1.readValue() && car.program_running)
            ;
        reset(1);
    }
}