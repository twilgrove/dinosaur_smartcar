#include "headfile.h"

void Update_motor()
{
    if (car.car_running == 0)
    {
        rp_duty = 0;
        lp_duty = 0;
        r_target = 0;
        l_target = 0;
        rp.set_duty(rp_duty);
        lp.set_duty(lp_duty);
        return;
    }
    else
    {
        r_now = static_cast<float>(std::abs(right_encoder.pulse_counter_update()));
        l_now = static_cast<float>(std::abs(left_encoder.pulse_counter_update()));

        if (rp_pid.apply_deadzone(r_target, WHEEL_SPEED_DEADBAND))
        {
            rp_duty = rp_pid.get(r_target, r_now) * WHEEL_SPEED_PID_KP;
        }
        else
        {
            rp_pid.reset();
            r_target = 0;
            rp_duty = 0;
        }

        if (lp_pid.apply_deadzone(l_target, WHEEL_SPEED_DEADBAND))
        {
            lp_duty = lp_pid.get(l_target, l_now) * WHEEL_SPEED_PID_KP;
        }
        else
        {
            lp_pid.reset();
            l_target = 0;
            lp_duty = 0;
        }

        rp_duty = MAX_OUTPUT_LIMIT(rp_duty, WHEEL_MAX_PLUS_ns);
        lp_duty = MAX_OUTPUT_LIMIT(lp_duty, WHEEL_MAX_PLUS_ns);
        lp.set_duty(lp_duty);
        rp.set_duty(rp_duty);
    }
}

void Update_servo()
{
    sp_duty = MIN_OUTPUT_LIMIT(sp_duty, SERVO_MIN_PLUS_ns);
    sp_duty = MAX_OUTPUT_LIMIT(sp_duty, SERVO_MAX_PLUS_ns);
    sp.set_duty(sp_duty);
}

void IMU_Camera_Check()
{

    if (car.Camera_running && car.IMU_running)
        return;
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
void Update_ips()
{
    ips200_clear();
    ips200_show_string(10, 10, "Program_state:running");
    ips200_show_string(10, 30, "car_state:");
    ips200_show_uint(120, 30, car.car_running, 1);
    ips200_show_string(10, 50, "IMG_send:");
    ips200_show_uint(120, 50, car.IMG_send, 1);
    ips200_show_string(10, 70, "IMG_display:");
    ips200_show_uint(120, 70, car.IMG_display, 1);
}