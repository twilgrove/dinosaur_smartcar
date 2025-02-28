/*
 * 文件名: pid.c
 * 日期: 2025.1.24
 * 作者: T.Grove
 * 描述: PID控制器实现文件，包含位置式和增量式PID算法
 * 版本: v0.0.1
 * 修改:
 *   - v1.0.0 (2025.1.24): 初步实现位置式PID算法和增量式PID算法，未经测试
 *   - v1.0.1 (2025.2.12): 优化了代码结构，添加了注释
 */

#include "pid.h"

/* 位置式PID初始化 */
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
    uint8_t d_lead)  // 微分先行使能
{
    PID_Config pid;

    // 初始化PID参数
    pid.type = PID_POSITION;
    pid.Kp = kp;
    pid.Ki = ki;
    pid.Kd = kd;
    pid.max_output = max_out;
    pid.min_output = min_out;
    pid.max_integral = max_iout;
    pid.min_integral = min_iout;
    pid.alpha = alpha;
    pid.deadband = deadband;
    pid.separation_threshold = threshold;
    pid.differential_lead = d_lead;

    // 初始化内部状态
    pid.prev_error = 0.0f;
    pid.prev_delta_error = 0.0f;
    pid.prev_measured_value = 0.0f;
    pid.integral = 0.0f;
    pid.last_output = 0.0f;
    pid.prev_derivative = 0.0f;

    // 初始化无用参数
    pid.max_delta_output = 0.0f;
    pid.min_delta_output = 0.0f;

    return pid;
}

/*增量式PID初始化*/
PID_Config PID_Init_Incremental(
    float kp, // 比例系数
    float ki, // 积分系数
    float kd, // 微分系数

    float max_out,       // PID输出上限
    float min_out,       // PID输出下限
    float max_delta_out, // 输出增量上限
    float min_delta_out, // 输出增量下限

    float deadband) // 死区范围
{
    PID_Config pid;

    // 初始化PID参数
    pid.type = PID_INCREMENT;
    pid.Kp = kp;
    pid.Ki = ki;
    pid.Kd = kd;
    pid.max_output = max_out;
    pid.min_output = min_out;
    pid.max_delta_output = max_delta_out;
    pid.min_delta_output = min_delta_out;
    pid.deadband = deadband;

    // 初始化内部状态
    pid.prev_error = 0.0f;
    pid.prev_delta_error = 0.0f;
    pid.prev_measured_value = 0.0f;
    pid.integral = 0.0f;
    pid.last_output = 0.0f;
    pid.prev_derivative = 0.0f;

    // 初始化无用参数
    pid.max_integral = 0.0f;
    pid.min_integral = 0.0f;
    pid.alpha = 0.0f;
    pid.separation_threshold = 0.0f;
    pid.differential_lead = 0;

    return pid;
}

/*
 * 功能: 计算PID控制器输出
 * 参数:
 *   pid - PID配置结构体指针
 *   setpoint - 设定值
 *   measured_value - 实际测量值
 * 返回: PID控制器的输出值
 * 说明:
 *   - 位置式PID公式: Output = Kp*e + Ki*∫e + Kd*Δe
 *   - 增量式PID公式: Output = Output_prev + ΔOutput
 *                   ΔOutput = Kp*(e-e_prev) + Ki*e + Kd*(Δe-Δe_prev)
 *   - 微分项采用一阶滞后滤波，积分分离和微分先行均支持两种模式
 */
float PID_Get(PID_Config *pid, float setpoint, float measured_value)
{
    /*================= 公共计算部分 =================*/

    float output = 0.0f;                         // 输出
    float error = setpoint - measured_value;     // 当前误差
    float delta_error = error - pid->prev_error; // 误差变化量
    float current_derivative = 0.0f;             // 当前微分值
    // 死区处理
    if (fabs(error) < pid->deadband)
        error = 0.0f;

    /*================= 算法分支处理 =================*/

    if (pid->type == PID_POSITION)
    {
        /*---------- 位置式PID计算 ----------*/
        /* 比例项处理 */
        float proportional = pid->Kp * error;

        /* 积分项处理 */
        if (!pid->separation_threshold || fabs(error) <= pid->separation_threshold)
        {
            // 积分累加并限幅
            pid->integral += pid->Ki * error;
            pid->integral = fmaxf(fminf(pid->integral, pid->max_integral), pid->min_integral);
        }

        /* 微分项处理 */
        if (pid->differential_lead)
            delta_error = pid->prev_measured_value - measured_value; // 微分先行：使用测量值的变化率

        // 应用微分滤波 (一阶滞后滤波)
        current_derivative = pid->Kd * delta_error;
        current_derivative = pid->alpha * pid->prev_derivative + (1 - pid->alpha) * current_derivative;

        output = proportional + pid->integral + current_derivative;
    }
    else
    {
        /*---------- 增量式PID核心计算 ----------*/
        // 微分项处理
        current_derivative = pid->Kd * (delta_error - pid->prev_delta_error);

        // 计算增量
        float delta_output = pid->Kp * delta_error +
                             pid->Ki * error +
                             current_derivative;

        // 输出增量限幅
        delta_output = fmaxf(fminf(delta_output, pid->max_delta_output), pid->min_delta_output);

        // 输出合成
        output = pid->last_output + delta_output;
    }

    /*============== 后处理 ==============*/
    // 输出限幅
    output = fmaxf(fminf(output, pid->max_output), pid->min_output);

    /* 更新状态 */
    pid->prev_measured_value = measured_value;
    pid->last_output = output;
    pid->prev_error = error;
    pid->prev_delta_error = delta_error;
    pid->prev_derivative = current_derivative;

    return output;
}

/*
 * 功能: 重置PID控制器内部状态
 * 参数: pid - PID配置结构体指针
 * 返回: 无
 * 说明: 清除PID控制器的历史状态，包括积分项、上一次误差等
 */
void PID_Reset(PID_Config *pid)
{
    /* 重置所有历史状态 */
    pid->prev_error = 0.0f;
    pid->prev_delta_error = 0.0f;
    pid->prev_measured_value = 0.0f;
    pid->integral = 0.0f;
    pid->prev_derivative = 0.0f;
    pid->last_output = 0.0f;
}