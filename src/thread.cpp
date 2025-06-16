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

        std::this_thread::sleep_for(std::chrono::milliseconds(CAR_THREAD_PERIOD));
    }
}

void l_debug_thread()
{
    static Timer t1000ms(L_DEBUG_THREAD_PERIOD, 1000);
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

        if (t1000ms.tick())
        {
            // std::cout << "fps: " << (uint32_t)car.cam_frame << std::endl;
            car.cam_frame = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(L_DEBUG_THREAD_PERIOD));
    }
}

void IO_thread()
{
    while (car.program_running)
    {

        if (key1.readValue())
        {
            std::cout << "key-1..." << std::endl;
            reset(1);
        }
        if (key2.readValue())
        {
            std::cout << "key-2..." << std::endl;
        }
        if (key3.readValue())
        {
            std::cout << "key-3..." << std::endl;
            car.IMG_send = !car.IMG_send;
            Update_ips();
        }
        if (key4.readValue())
        {
            std::cout << "key-4..." << std::endl;
            car.IMG_display = !car.IMG_display;
            Update_ips();
        }

        switch1_value = switch1.readValue();
        switch2_value = switch2.readValue();

        std::this_thread::sleep_for(std::chrono::milliseconds(IO_THREAD_PERIOD));
    }
}
