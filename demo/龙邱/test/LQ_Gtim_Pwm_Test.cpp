#include "main.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GtimPwmTest()
 * @功能说明：Gtim PWM 测试(寄存器)
 * @参数说明：无
 * @函数返回：无
 * @调用方法：GtimPwmTest();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GtimPwmTest()
{
    // 初始化 PWM，确认引脚号、通道号、极性、周期、占空比、引脚复用模式(可选)
    GtimPwm ServoPWM1(88, 2, LS_GTIM_INVERSED, 2000000 - 1, 150000);
    GtimPwm ServoPWM2(89, 3, LS_GTIM_NORMAL, 2000000 - 1, 150000);
    // 启动 PWM
    ServoPWM1.Enable();
    ServoPWM2.Enable();
    while(1)
    {
        ServoPWM1.SetDutyCycle(100000);
        ServoPWM2.SetDutyCycle(100000);
        printf("Gtim PWM Set %d\n", 100000);
        sleep(1);
        ServoPWM1.SetDutyCycle(500000);
        ServoPWM2.SetDutyCycle(500000);
        printf("Gtim PWM Set %d\n", 500000);
        sleep(1);
        ServoPWM1.SetDutyCycle(900000);
        ServoPWM2.SetDutyCycle(900000);
        printf("Gtim PWM Set %d\n", 900000);
        sleep(1);
    }
}
