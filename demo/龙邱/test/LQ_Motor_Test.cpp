#include "main.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void MotorTest()
 * @功能说明：电机功能测试
 * @参数说明：无
 * @函数返回：无
 * @调用方法：MotorTest();
 * @备注说明：使电机每隔 2 秒转动方向翻转一次
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void MotorTest()
{
    /* 电机PWM */
    SetPWM LMotorPWM(PWM1, 100000, 10, "inversed"); // TIM2_CH1 GPIO87
    SetPWM RMotorPWM(PWM2, 100000, 10, "inversed"); // TIM2_CH2 GPIO88
    /* 电机方向GPIO */
    HWGpio LMotoDir(74, GPIO_Mode_Out); // CAN3_RX  left motor dir
    HWGpio RMotoDir(75, GPIO_Mode_Out); // CAN3_TX  right motor dir
    // 设置占空比
    LMotorPWM.SetDutyCycle(30000);
    RMotorPWM.SetDutyCycle(30000);
    // 使能
    LMotorPWM.Enable();
    RMotorPWM.Enable();

    while(true)
    {
        LMotoDir.SetGpioValue(1);
        RMotoDir.SetGpioValue(1);
        sleep(2);
        LMotoDir.SetGpioValue(0);
        RMotoDir.SetGpioValue(0);
        sleep(2);
    }
}
