/*
 * 文件名: pid.cpp
 * 日期: 2025.2.27
 * 作者: T.Grove
 * 描述: PID控制器实现文件，支持位置式和增量式PID算法
 * 版本: v1.0.0
 * 修改:
 *   - v1.0.0 (2025.2.27): 初始版本，实现基础的位置式和增量式PID算法
 */

#include "pid.h"

// 位置式PID初始化构造函数
pid::pid(Mode mode, float kp, float ki, float kd, float abs_process, float max_output, float min_output)
{
    mode_ = mode;
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    if (mode == Mode::POSITION) // 位置式PID,积分限幅
    {
        this->max_integral = abs_process;
        this->min_integral = -abs_process;
    }
    else // 增量式PID,输出增量限幅
    {
        this->max_delta_output = abs_process;
        this->min_delta_output = -abs_process;
    }
    this->max_output = max_output;
    this->min_output = min_output;
}

float pid::get(float set_value, float now_value)
{
    /*================= 公共计算部分 =================*/

    float output = 0.0f;                    // 输出
    float error = set_value - now_value;    // 当前误差
    float delta_error = error - prev_error; // 误差变化量
    float current_derivative = 0.0f;        // 当前微分值
    // 死区处理
    if (fabs(error) < deadband)
        error = 0.0f;

    /*================= 算法分支处理 =================*/

    if (mode_ == Mode::POSITION)
    {
        /*---------- 位置式PID计算 ----------*/
        /* 比例项处理 */
        float proportional = kp * error;

        /* 积分项处理 */
        if (!integral_separation || fabs(error) <= integral_separation)
        {
            // 积分累加并限幅
            integral += ki * error;
            integral = fmaxf(fminf(integral, max_integral), min_integral);
        }

        /* 微分项处理 */
        if (differential_lead)
            delta_error = prev_measured_value - now_value; // 微分先行：使用测量值的变化率

        // 应用微分滤波 (一阶滞后滤波)
        current_derivative = kd * delta_error;
        current_derivative = alpha * prev_derivative + (1 - alpha) * current_derivative;

        output = proportional + integral + current_derivative;
    }
    else
    {
        /*---------- 增量式PID核心计算 ----------*/
        // 微分项处理
        current_derivative = kd * (delta_error - prev_delta_error);

        // 计算增量
        float delta_output = kp * delta_error +
                             ki * error +
                             current_derivative;

        // 输出增量限幅
        delta_output = fmaxf(fminf(delta_output, max_delta_output), min_delta_output);

        // 输出合成
        output = last_output + delta_output;
    }

    /*============== 后处理 ==============*/
    // 输出限幅
    output = fmaxf(fminf(output, max_output), min_output);

    /* 更新状态 */
    prev_measured_value = now_value;
    last_output = output;
    prev_error = error;
    prev_delta_error = delta_error;
    prev_derivative = current_derivative;

    return output;
}

void pid::set_kp(float kp)
{
    this->kp = kp;
}

void pid::set_ki(float ki)
{
    this->ki = ki;
}

void pid::set_kd(float kd)
{
    this->kd = kd;
}

void pid::set_integral(float min_integral, float max_integral)
{
    this->min_integral = min_integral;
    this->max_integral = max_integral;
}

void pid::set_delta_output(float min_delta_output, float max_delta_output)
{
    this->min_delta_output = min_delta_output;

    this->max_delta_output = max_delta_output;
}
void pid::set_output(float min_output, float max_output)
{
    this->min_output = min_output;
    this->max_output = max_output;
}

void pid::set_deadband(float deadband)
{
    this->deadband = deadband;
}

void pid::set_alpha(float alpha)
{
    this->alpha = alpha;
}

void pid::set_integral_separation(float integral_separation)
{
    this->integral_separation = integral_separation;
}

void pid::set_differential_lead(bool differential_lead)
{
    this->differential_lead = differential_lead;
}

void pid::reset()
{
    this->integral = 0;
    this->prev_error = 0;
    this->prev_delta_error = 0;
    this->prev_measured_value = 0;
    this->prev_derivative = 0;
    this->last_output = 0;
}
