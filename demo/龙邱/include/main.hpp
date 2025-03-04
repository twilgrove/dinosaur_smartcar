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

@修改日期：2025-02-26
@修改内容：
@注意事项：注意查看路径的修改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#pragma once

#include <iostream>
#include <string>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>    // 共享内存
#include <sys/sem.h>    // 信号量集

#include <opencv2/opencv.hpp>

#include "LQ_PWM.hpp"
#include "LQ_GTIM_PWM.hpp"
#include "LQ_GPIO.hpp"
#include "LQ_HW_GPIO.hpp"
#include "LQ_PWM_ENCODER.hpp"
#include "LQ_TFT.hpp"
#include "LQ_HW_PWM.hpp"
#include "LQ_HW_SPI.hpp"

using namespace cv;
using namespace std;

// 电机测试程序
void MotorTest();

// 舵机测试程序
void ServoTest();

// 久久派 21 个 GPIO 翻转测试
void GpioTest();

// GPIO 输出测试(设备文件)
void GpioOutputTest1(int gpio);

// GPIO 输出测试(硬件)
void GpioOutputTest2(int gpio);

// GPIO 输入测试(设备文件)
void GpioInputTest1();

// GPIO 输入测试(硬件)
void GpioInputTest2();

// PWM 测试(设备文件)
void PwmDevTest();

// PWM 测试(寄存器)
void PwmHWTest();

// Gtim PWM 测试(硬件)
void GtimPwmTest();

// 编码器测试(硬件)
void EncoderTest();

// 摄像头测试
void CameraTest();

// ADC 功能测试
void AdcFunTest();

// tft屏幕测试程序
void TFTTest();

// sleep()函数测试 -- 以秒为单位延时
void sleepTest();

// usleep()函数测试 -- 以微秒为单位延时
void usleepTest();

// nanosleep()函数测试 -- 以纳秒为单位延时
void nanosleepTest();

// clock_nanosleep()函数测试 -- 以纳秒为单位延时
void clock_nanosleepTest();

// 计算帧率
void CalculateFrameRate();

// 时间戳打印测试
void GetTimeTest();

// 获取当前毫秒值(日历时间)
void GetCurrentMillisecond();

// 打印当前微秒值
void printMicTimestamp();
