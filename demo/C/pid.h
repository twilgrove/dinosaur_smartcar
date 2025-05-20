/*
 * 文件名: pid.h
 * 日期: 2025.1.24
 * 作者: T.Grove
 * 描述: PID控制器头文件，支持位置式和增量式PID算法
 * 版本: v1.0.0
 * 修改:
 *   - v1.0.0 (2025.1.24): 初始版本，实现基础的位置式和增量式PID算法
 *   - v1.0.1 (2025.2.12): 优化了代码结构，添加了注释
 */

#ifndef __PID_H
#define __PID_H

#include <stdint.h>
#include <math.h>
/*
 * PID控制器使用说明：
 *
 * 1. 初始化流程
 *    在pid.c的PID_Init函数中定义配置PID参数并返回PID_Config结构体
 *
 * 2. 运行使用流程
 *    while (1) {
 *        float setpoint = get_target();     // 获取目标值，此函数仅为示例，具体情况请自行实现
 *        float measured = get_sensor();     // 获取测量值，此函数仅为示例，具体情况请自行实现
 *        float output = PID_Get(&pid, setpoint, measured);  // 计算PID输出
 *        set_actuator(output);              // 设置执行器输出，此函数仅为示例，具体情况请自行实现
 *    }
 *
 * 3. 注意事项
 *    - 使用位置式PID时必须设置积分限幅参数，
 *      对于响应较快的系统（如电机控制）：积分限幅一般设置为输出限幅的 30% ~ 50%。
 *      对于响应较慢的系统（如温度控制）：积分限幅一般设置为输出限幅的 50% ~ 70%。
 *    - 所有内部状态变量必须初始化为0
 *    - 优化参数根据实际需求配置，不需要时可设置为0
 *
 * 舵机:位置式PD
 * 电机:增量式PID
 */

/* PID算法类型枚举 */
typedef enum
{
    PID_POSITION = 0, // 位置式PID
    PID_INCREMENT = 1 // 增量式PID
} PID_Type;

/* PID参数结构体 */
typedef struct
{
    /* 算法选择 */
    PID_Type type; // [通用] PID算法类型：0-位置式，1-增量式

    /* 基础参数 */
    float Kp; // [通用] 比例系数 - 控制系统对偏差的响应强度，不能为0
    float Ki; // [通用] 积分系数 - 消除稳态误差，可以为0
    float Kd; // [通用] 微分系数 - 抑制超调，可以为0

    /* 限幅参数 */
    float max_delta_output; // [增量式] 输出增量上限 - 必须设置且大于min_delta_output
    float min_delta_output; // [增量式] 输出增量下限 - 必须设置且小于max_delta_output
    float max_integral;     // [位置式] 积分项上限 - 必须大于min_integral
    float min_integral;     // [位置式] 积分项下限 - 必须小于max_integral
    float max_output;       // [通用] PID输出上限 - 必须设置且大于min_output
    float min_output;       // [通用] PID输出下限 - 必须设置且小于max_output

    /* 优化参数 */
    float alpha;                // [位置式] 微分滤波系数 - 范围0-1，不使用设为0
    float deadband;             // [通用] 死区范围 - 不使用设为0
    float separation_threshold; // [位置式] 积分分离阈值 - 不使用设为0
    uint8_t differential_lead;  // [位置式] 微分先行使能 - 1:启用 0:禁用，不使用设为0

    /* 内部状态 */
    float prev_error;          // [通用] 上一次误差 - 初始化必须置0
    float prev_delta_error;    // [通用] 上一次误差变化量 - 初始化必须置0
    float prev_measured_value; // [通用] 上一次测量值 - 初始化必须置0
    float prev_derivative;     // [通用] 上一次微分值 - 初始化必须置0
    float integral;            // [位置式] 积分项 - 初始化必须置0
    float last_output;         // [增量式] 上一次输出值 - 初始化必须置0

} PID_Config;

/* 函数声明 */
PID_Config PID_Init_Position(
    float kp, // 比例系数
    float ki, // 积分系数
    float kd, // 微分系数

    float max_out,  // PID输出上限
    float min_out,  // PID输出下限
    float max_iout, // 积分项上限
    float min_iout, // 积分项下限

    float alpha,     // 微分滤波系数
    float deadband,  // 死区范围
    float threshold, // 积分分离阈值
    uint8_t d_lead); // 微分先行使能

PID_Config PID_Init_Incremental(
    float kp, // 比例系数
    float ki, // 积分系数
    float kd, // 微分系数

    float max_out,       // PID输出上限
    float min_out,       // PID输出下限
    float max_delta_out, // 输出增量上限
    float min_delta_out, // 输出增量下限

    float deadband); // 死区范围

void PID_Reset(PID_Config *pid);                                      // 重置PID控制器内部状态
float PID_Get(PID_Config *pid, float setpoint, float measured_value); // 计算PID输出

#endif // __PID_H