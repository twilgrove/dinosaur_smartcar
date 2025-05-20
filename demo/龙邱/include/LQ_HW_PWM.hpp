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

#include "LQ_MAP_ADDR.hpp"

using namespace std;

/***********************************寄存器地址**************************************/
#define LS_PWM_BASE_ADDR            0x1611B000  // pwm0基地址
#define LS_PWM_OFFSET               0x10        // pwm控制器偏移量
#define LS_PWM_LOW_BUFFER_OFFSET    0x4         // 低脉冲缓冲寄存器
#define LS_PWM_FULL_BUFFER_OFFSET   0x8         // 脉冲周期缓冲寄存器
#define LS_PWM_CTRL_OFFSET          0xC         // 控制寄存器

/********************************控制寄存器各位域***********************************/
#define LS_PWM_CTRL_EN      BIT(0)  // 计数器使能   （1--计数；0--停止）
#define LS_PWM_CTRL_OE      BIT(3)  // 脉冲输出使能控制位（1--失能；0--使能）
#define LS_PWM_CTRL_SINGLE  BIT(4)  // 单脉冲控制位（1--单；0--持续）
#define LS_PWM_CTRL_INTE    BIT(5)  // 中断使能     （1--使能；0--失能）
#define LS_PWM_CTRL_INT     BIT(6)  // 中断位（读：1--有中断；0--没有。写：1--清中断）
#define LS_PWM_CTRL_RST     BIT(7)  // 计数器重置   （1--重置；0--正常工作）
#define LS_PWM_CTRL_CAPTE   BIT(8)  // 测量脉冲使能 （1--测量脉冲；0--脉冲输出）
#define LS_PWM_CTRL_INVERT  BIT(9)  // 输出翻转使能（1--翻转；0--不翻转）
#define LS_PWM_CTRL_DZONE   BIT(10) // 防死区（1--开启；0--关闭）

#define PWM_CLK_FRE     100000000L  // PWM 控制器时钟周期

/* 选择 PWM 极性的宏 */
#define LS_PWM_NORMAL   0   // 正相
#define LS_PWM_INVERSED 1   // 反相

/* 选择对应 PWM 通道的宏 */
#define LS_PWM0 0
#define LS_PWM1 1
#define LS_PWM2 2
#define LS_PWM3 3

typedef unsigned long long uint_ll;

class HWPwm
{
public:
    HWPwm();    // 构造函数
    HWPwm(uint8_t gpio, uint8_t chNum, uint8_t pol, uint32_t period, uint32_t duty_cycle, int mux = 0b01);
    void Init();                            // 初始化函数
    void Enable();                          // 使能
    void Disable();                         // 失能
    void SetPolarity(uint8_t pol);          // 设置极性
    void SetPeriod(uint32_t period);        // 设置周期
    void SetDutyCycle(uint32_t duty_cycle); // 设置占空比
    ~HWPwm();   // 析构函数
public:
    uint8_t  Gpio;          // 引脚号
    uint8_t  ChNum;         // 通道号
    uint8_t  Pol;           // 极性
    uint32_t Period;        // 周期
    uint32_t Duty_cycle;    // 占空比
private:
    uint32_t PWM_ADDR;  // 对应 PWMx 基地址
    void *Low_buffer;   // 低脉冲缓冲寄存器
    void *Full_buffer;  // 脉冲周期缓冲寄存器
    void *CTRL;         // 控制寄存器
};
