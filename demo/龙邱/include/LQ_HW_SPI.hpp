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

@�޸����ڣ�2025-03-04
@�޸����ݣ�
@ע�����ע��鿴·�����޸�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#pragma once

#include <linux/spi/spidev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

class HW_SPI
{
public:
    HW_SPI(); // ���캯��
    HW_SPI(const char *file, 
           uint8_t mode = SPI_MODE_0,
           uint8_t bits = 8,
           uint32_t speed = 50000000);              // ���ι��캯��
    void SetMode(uint8_t mode);                     // ���� SPI ģʽ
    void SetBits(uint8_t bits);                     // �����շ�����λ��
    void SetSpeed(uint32_t speed);                  // ���ô����ٶ�
    void SPI_Write(uint8_t tx[], uint8_t tx_Num);   // ��������
    ~HW_SPI(); // ��������
public:
    int spi_fd; // ���spi�豸�ļ����ļ�������
private:
    uint8_t  Mode;  // ģʽ
    uint8_t  Bits;  // �ֽ���
    uint32_t Speed; // �ٶ�
    struct spi_ioc_transfer Tr;
};
