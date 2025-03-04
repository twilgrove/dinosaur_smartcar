#include "main.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void EncoderTest()
 * @����˵��������������(�Ĵ�����PWM ������ʵ��)
 * @����˵������
 * @�������أ���
 * @���÷�����EncoderTest();
 * @��ע˵�������Դ�������Ҳ����ת�鿴����������ֵ
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void EncoderTest()
{
    // ���PWM
    SetPWM LMotorPWM(PWM2, 100000, 32000, "inversed");
    SetPWM RMotorPWM(PWM1, 100000, 32000, "inversed");
    cout << "EncoderTest1" << endl;
    // �������GPIO
    HWGpio LMotoDir(72, GPIO_Mode_Out);
    HWGpio RMotoDir(73, GPIO_Mode_Out);
    cout << "EncoderTest2" << endl;
    // ��ʼ������������ͨ��
    LS_PwmEncoder EncodersL(0, 71);
    LS_PwmEncoder EncodersR(3, 70);
    cout << "EncoderTest3" << endl;
    // ��ʼ�����
    LMotorPWM.Enable();
    RMotorPWM.Enable();
    cout << "EncoderTest4" << endl;
    while (1)
    {
        cout << "left = " << EncodersL.Update() << ", right = " << EncodersR.Update() << endl;
        usleep(1000);
    }
}
