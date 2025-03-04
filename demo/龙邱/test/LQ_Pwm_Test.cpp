#include "main.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void PwmDevTest()
 * @����˵����PWM �������� PWM ����(�豸�ļ�)
 * @����˵������
 * @�������أ���
 * @���÷�����PwmDevTest();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void PwmDevTest()
{
    // ���� PWM
    SetPWM pwm1(PWM1, 500, 250, PWM_INVERSED);
    SetPWM pwm2(PWM2, 10000, 5000, PWM_INVERSED);
    // ���� PWM
    pwm1.Enable();
    pwm2.Enable();
    while(1)
    {
        pwm1.SetDutyCycle(100);
        pwm2.SetDutyCycle(1000);
        printf("Set PWM 2500\n");
        sleep(1);
        pwm1.SetDutyCycle(250);
        pwm2.SetDutyCycle(5000);
        printf("Set PWM 5000\n");
        sleep(1);
        pwm1.SetDutyCycle(400);
        pwm2.SetDutyCycle(9000);
        printf("Set PWM 7500\n");
        sleep(1);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void PwmHWTest()
 * @����˵����PWM �������� PWM ����(�Ĵ���)
 * @����˵������
 * @�������أ���
 * @���÷�����PwmHWTest();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void PwmHWTest()
{
    // ���� PWM
    HWPwm pwm0(64, LS_PWM0, LS_PWM_INVERSED, 50, 5);
    HWPwm pwm1(65, LS_PWM1, LS_PWM_INVERSED, 100, 10);
    HWPwm pwm2(66, LS_PWM2, LS_PWM_INVERSED, 1000, 100);
    HWPwm pwm3(67, LS_PWM3, LS_PWM_INVERSED, 10000, 5000);
    // ���� PWM
    pwm0.Enable();
    pwm1.Enable();
    pwm2.Enable();
    pwm3.Enable();
    while(1)
    {
        pwm0.SetDutyCycle(5);
        pwm1.SetDutyCycle(10);
        pwm2.SetDutyCycle(100);
        pwm3.SetDutyCycle(1000);
        printf("Set HW PWM 1000\n");
        sleep(1);
        pwm0.SetDutyCycle(25);
        pwm1.SetDutyCycle(50);
        pwm2.SetDutyCycle(500);
        pwm3.SetDutyCycle(5000);
        printf("Set HW PWM 5000\n");
        sleep(1);
        pwm0.SetDutyCycle(45);
        pwm1.SetDutyCycle(90);
        pwm2.SetDutyCycle(900);
        pwm3.SetDutyCycle(9000);
        printf("Set HW PWM 9000\n");
        sleep(1);
    }
}
