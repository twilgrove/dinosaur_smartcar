#include "main.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void MotorTest()
 * @����˵����������ܲ���
 * @����˵������
 * @�������أ���
 * @���÷�����MotorTest();
 * @��ע˵����ʹ���ÿ�� 2 ��ת������תһ��
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void MotorTest()
{
    /* ���PWM */
    SetPWM LMotorPWM(PWM1, 100000, 10, "inversed"); // TIM2_CH1 GPIO87
    SetPWM RMotorPWM(PWM2, 100000, 10, "inversed"); // TIM2_CH2 GPIO88
    /* �������GPIO */
    HWGpio LMotoDir(74, GPIO_Mode_Out); // CAN3_RX  left motor dir
    HWGpio RMotoDir(75, GPIO_Mode_Out); // CAN3_TX  right motor dir
    // ����ռ�ձ�
    LMotorPWM.SetDutyCycle(30000);
    RMotorPWM.SetDutyCycle(30000);
    // ʹ��
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
