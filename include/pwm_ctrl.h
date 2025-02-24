/*
 * 文件名: pwm_ctrl.h
 * 日期: 2025.02.07
 * 作者: T.Grove
 * 描述: PWM控制模块头文件，提供PWM配置和控制相关功能
 * 版本: v1.0.0
 * 修改:
 *   - v1.0.0 (2025.02.07): 初始版本
 */

#ifndef __PWM_CTRL_H
#define __PWM_CTRL_H

/* 外部依赖 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

/* 宏定义 */
#define PWM_ID_MAX 6 // 支持的最大PWM通道数

#ifdef __cplusplus
extern "C"
{
#endif

    /* 类型定义 */
    typedef struct
    {
        uint8_t fd;                 // pwm文件描述符
        uint8_t fd_pwm_duty;        // pwm占空比文件描述符
        uint32_t period_ns;         // pwm周期(纳秒)
        uint32_t duty_cycle_ns;     // pwm占空比(纳秒)
        uint8_t pwmchip_number;     // pwmchip编号
        uint8_t pwmchip_pwm_number; // pwm编号
    } PWM_Config;

    /* 函数声明 */
    void pwm_init(uint8_t pwm_id, uint8_t pwmchip_number, uint8_t pwmchip_pwm_number, uint32_t period_ns); // 初始化PWM通道
    void pwm_set_period(uint8_t pwm_id, uint32_t period_ns);                                               // 设置PWM周期
    void pwm_set_duty_cycle(uint8_t pwm_id, float duty_cycle_percent);                                     // 设置PWM占空比(百分比)
    void pwm_enable_or_disable(uint8_t pwm_id, uint8_t enable);                                            // 使能或禁用PWM输出
    void pwm_remove(uint8_t pwm_id);                                                                       // 移除PWM通道配置

#ifdef __cplusplus
}
#endif

#endif /* __PWM_CTRL_H */
