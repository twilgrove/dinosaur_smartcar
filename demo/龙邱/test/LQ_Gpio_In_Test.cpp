#include "main.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void GpioInputTest1()
 * @����˵����GPIO ���빦�ܲ���(�豸�ļ�)
 * @����˵������
 * @�������أ���
 * @���÷�����GpioInputTest1();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GpioInputTest1()
{
    SetGPIO Gpio1(44, "in");
    SetGPIO Gpio2(45, "in");
    while(1)
    {
        if(Gpio1.GetGpioValue() == 0){
            printf("Button 0 is pressed\n");
            usleep(500000);
        }
        if(Gpio2.GetGpioValue() == 0){
            printf("Button 1 is pressed\n");
            usleep(500000);
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void GpioInputTest2()
 * @����˵����GPIO ���빦�ܲ���(�Ĵ���)
 * @����˵������
 * @�������أ���
 * @���÷�����GpioInputTest2();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GpioInputTest2()
{
    HWGpio Gpio1(44, GPIO_Mode_In);
    HWGpio Gpio2(45, GPIO_Mode_In);
    while(1)
    {
        if(Gpio1.GetGpioValue() == 0){
            printf("Button 0 is pressed\n");
            usleep(500000);
        }
        if(Gpio2.GetGpioValue() == 0){
            printf("Button 1 is pressed\n");
            usleep(500000);
        }
    }
}
