/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编   写：龙邱科技
@邮   箱：chiusir@163.com
@编译IDE：Linux 环境、VSCode_1.93 及以上版本、Cmake_3.16 及以上版本
@使用平台：龙芯2K0300久久派和北京龙邱智能科技龙芯久久派拓展板
@相关信息参考下列地址
    网      站：http://www.lqist.cn
    淘 宝 店 铺：http://longqiu.taobao.com
    程序配套视频：https://space.bilibili.com/95313236
@软件版本：V1.0 版权所有，单位使用请先联系授权
@参考项目链接：https://github.com/AirFortressIlikara/ls2k0300_peripheral_library

@修改日期：2025-03-04
@修改内容：
@注意事项：注意查看路径的修改
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
    HW_SPI(); // 构造函数
    HW_SPI(const char *file, 
           uint8_t mode = SPI_MODE_0,
           uint8_t bits = 8,
           uint32_t speed = 50000000);              // 带参构造函数
    void SetMode(uint8_t mode);                     // 设置 SPI 模式
    void SetBits(uint8_t bits);                     // 设置收发数据位数
    void SetSpeed(uint32_t speed);                  // 设置传输速度
    void SPI_Write(uint8_t tx[], uint8_t tx_Num);   // 发送数据
    ~HW_SPI(); // 析构函数
public:
    int spi_fd; // 存放spi设备文件的文件描述符
private:
    uint8_t  Mode;  // 模式
    uint8_t  Bits;  // 字节数
    uint32_t Speed; // 速度
    struct spi_ioc_transfer Tr;
};
