#include "main.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void GpioOutputTest1()
 * @����˵����GPIO ������ܲ���(�豸�ļ�)
 * @����˵������
 * @�������أ���
 * @���÷�����GpioOutputTest1();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GpioOutputTest1(int gpio)
{
    SetGPIO Gpio(gpio, "out");
    while(1)
    {
        Gpio.SetGpioValue(0);
        printf("Set gpio value 0\n");
        sleep(1);
        Gpio.SetGpioValue(1);
        printf("Set gpio value 1\n");
        sleep(1);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void GpioOutputTest2()
 * @����˵����GPIO ������ܲ���(�Ĵ���)
 * @����˵������
 * @�������أ���
 * @���÷�����GpioOutputTest2();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GpioOutputTest2(int gpio)
{
    HWGpio Gpio(gpio, GPIO_Mode_Out);
    while(1)
    {
        Gpio.SetGpioValue(0);
        printf("Set HWGpio value 0\n");
        sleep(1);
        Gpio.SetGpioValue(1);
        printf("Set HWGpio value 1\n");
        sleep(1);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void GpioTest()
 * @����˵�����þ���22��GPIO��ת����
 * @����˵������
 * @�������أ���
 * @���÷�����GpioTest();
 * @��ע˵�������ʹ���������֮�󣬰����ϵ��豸�ļ����޷�����ʹ�ã��������þ���
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GpioTest()
{
    #define GPIONUMS 22
    int gpioNum[GPIONUMS] = {88, 89, 73, 72, 75, 74, 50, 51, 64, 65, 66, 67, 60, 62, 63, 42, 43, 44, 45, 48, 49, 61};
    HWGpio gpio[GPIONUMS];
    for (int i = 0; i < GPIONUMS; i++)
    {
        gpio[i] = HWGpio(gpioNum[i], GPIO_Mode_Out);
    }
    while (1)
    {
        for (int i = 0; i < GPIONUMS; i++)
        {
            gpio[i].SetGpioValue(1);
        }
        printf("Set gpio H\n");
        sleep(1);
        for (int i = 0; i < GPIONUMS; i++)
        {
            gpio[i].SetGpioValue(0);
        }
        printf("Set gpio L\n");
        sleep(1);
    }
}
