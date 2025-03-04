#include "main.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void GtimPwmTest()
 * @����˵����Gtim PWM ����(�Ĵ���)
 * @����˵������
 * @�������أ���
 * @���÷�����GtimPwmTest();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GtimPwmTest()
{
    // ��ʼ�� PWM��ȷ�����źš�ͨ���š����ԡ����ڡ�ռ�ձȡ����Ÿ���ģʽ(��ѡ)
    GtimPwm ServoPWM1(88, 2, LS_GTIM_INVERSED, 2000000 - 1, 150000);
    GtimPwm ServoPWM2(89, 3, LS_GTIM_NORMAL, 2000000 - 1, 150000);
    // ���� PWM
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
