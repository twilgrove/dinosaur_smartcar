/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@��   д������Ƽ�
@��   �䣺chiusir@163.com
@����IDE��Linux ������VSCode_1.93 �����ϰ汾��Cmake_3.16 �����ϰ汾
@ʹ��ƽ̨����о2K0300�þ��ɺͱ����������ܿƼ���о�þ�����չ��
@�����Ϣ�ο����е�ַ
    ��      վ��http://www.lqist.cn
    �� �� �� �̣�http://longqiu.taobao.com
    ����������Ƶ��https://space.bilibili.com/95313236
@����汾��V1.0 ��Ȩ���У���λʹ��������ϵ��Ȩ
@�ο���Ŀ���ӣ�https://github.com/AirFortressIlikara/ls2k0300_peripheral_library

@�޸����ڣ�2025-02-26
@�޸����ݣ�
@ע�����ע��鿴·�����޸�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#pragma once

#include <iostream>

#include "LQ_MAP_ADDR.hpp"

using namespace std;

/***********************************�Ĵ�����ַ**************************************/
#define LS_PWM_BASE_ADDR            0x1611B000  // pwm0����ַ
#define LS_PWM_OFFSET               0x10        // pwm������ƫ����
#define LS_PWM_LOW_BUFFER_OFFSET    0x4         // �����建��Ĵ���
#define LS_PWM_FULL_BUFFER_OFFSET   0x8         // �������ڻ���Ĵ���
#define LS_PWM_CTRL_OFFSET          0xC         // ���ƼĴ���

/********************************���ƼĴ�����λ��***********************************/
#define LS_PWM_CTRL_EN      BIT(0)  // ������ʹ��   ��1--������0--ֹͣ��
#define LS_PWM_CTRL_OE      BIT(3)  // �������ʹ�ܿ���λ��1--ʧ�ܣ�0--ʹ�ܣ�
#define LS_PWM_CTRL_SINGLE  BIT(4)  // ���������λ��1--����0--������
#define LS_PWM_CTRL_INTE    BIT(5)  // �ж�ʹ��     ��1--ʹ�ܣ�0--ʧ�ܣ�
#define LS_PWM_CTRL_INT     BIT(6)  // �ж�λ������1--���жϣ�0--û�С�д��1--���жϣ�
#define LS_PWM_CTRL_RST     BIT(7)  // ����������   ��1--���ã�0--����������
#define LS_PWM_CTRL_CAPTE   BIT(8)  // ��������ʹ�� ��1--�������壻0--���������
#define LS_PWM_CTRL_INVERT  BIT(9)  // �����תʹ�ܣ�1--��ת��0--����ת��
#define LS_PWM_CTRL_DZONE   BIT(10) // ��������1--������0--�رգ�

#define PWM_CLK_FRE     100000000L  // PWM ������ʱ������

/* ѡ�� PWM ���Եĺ� */
#define LS_PWM_NORMAL   0   // ����
#define LS_PWM_INVERSED 1   // ����

/* ѡ���Ӧ PWM ͨ���ĺ� */
#define LS_PWM0 0
#define LS_PWM1 1
#define LS_PWM2 2
#define LS_PWM3 3

typedef unsigned long long uint_ll;

class HWPwm
{
public:
    HWPwm();    // ���캯��
    HWPwm(uint8_t gpio, uint8_t chNum, uint8_t pol, uint32_t period, uint32_t duty_cycle, int mux = 0b01);
    void Init();                            // ��ʼ������
    void Enable();                          // ʹ��
    void Disable();                         // ʧ��
    void SetPolarity(uint8_t pol);          // ���ü���
    void SetPeriod(uint32_t period);        // ��������
    void SetDutyCycle(uint32_t duty_cycle); // ����ռ�ձ�
    ~HWPwm();   // ��������
public:
    uint8_t  Gpio;          // ���ź�
    uint8_t  ChNum;         // ͨ����
    uint8_t  Pol;           // ����
    uint32_t Period;        // ����
    uint32_t Duty_cycle;    // ռ�ձ�
private:
    uint32_t PWM_ADDR;  // ��Ӧ PWMx ����ַ
    void *Low_buffer;   // �����建��Ĵ���
    void *Full_buffer;  // �������ڻ���Ĵ���
    void *CTRL;         // ���ƼĴ���
};
