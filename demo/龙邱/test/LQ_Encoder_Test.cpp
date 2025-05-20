#include "main.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void EncoderTest()
 * @功能说明：编码器测试(寄存器，PWM 控制器实现)
 * @参数说明：无
 * @函数返回：无
 * @调用方法：EncoderTest();
 * @备注说明：可以搭配电机，也可手转查看编码器返回值
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void EncoderTest()
{
    // 电机PWM
    SetPWM LMotorPWM(PWM2, 100000, 32000, "inversed");
    SetPWM RMotorPWM(PWM1, 100000, 32000, "inversed");
    cout << "EncoderTest1" << endl;
    // 电机方向GPIO
    HWGpio LMotoDir(72, GPIO_Mode_Out);
    HWGpio RMotoDir(73, GPIO_Mode_Out);
    cout << "EncoderTest2" << endl;
    // 初始化编码器计数通道
    LS_PwmEncoder EncodersL(0, 71);
    LS_PwmEncoder EncodersR(3, 70);
    cout << "EncoderTest3" << endl;
    // 初始化电机
    LMotorPWM.Enable();
    RMotorPWM.Enable();
    cout << "EncoderTest4" << endl;
    while (1)
    {
        cout << "left = " << EncodersL.Update() << ", right = " << EncodersR.Update() << endl;
        usleep(1000);
    }
}
