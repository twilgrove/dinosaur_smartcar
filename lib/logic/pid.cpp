/*
 * 文件名: pid.cpp
 * 日期: 2025.2.27
 * 作者: T.Grove
 * 描述: PID控制器实现文件，支持位置式和增量式PID算法
 * 版本: v1.1.0
 * 修改:
 *   - v1.0.0 (2025.2.27): 初始版本，实现基础的位置式和增量式PID算法
 *   - v1.1.0 (2025.5.8): 改进版本，增加平滑积分,并封装参数为结构体
 */

#include "pid.h"

// 位置式PID初始化构造函数
pid::pid(Mode mode, float kp, float ki, float kd, float abs_process, float min_output, float max_output)
{
    config.mode_ = mode;
    config.kp = kp;
    config.ki = ki;
    config.kd = kd;
    if (mode == Mode::POSITION) // 位置式PID,积分限幅
    {
        config.max_integral = abs_process;
        config.min_integral = -abs_process;
    }
    else // 增量式PID,输出增量限幅
    {
        config.max_delta_output = abs_process;
        config.min_delta_output = -abs_process;
    }
    config.max_output = max_output;
    config.min_output = min_output;
}

void pid::init(const PIDConfig &cfg)
{
    config = cfg;
}

float pid::get(float set_value, float now_value)
{
    /*================= 公共计算部分 =================*/

    float output = 0.0f;                    // 输出
    float error = set_value - now_value;    // 当前误差
    float delta_error = error - prev_error; // 误差变化量
    float current_derivative = 0.0f;        // 当前微分值
    // 死区处理
    if (my_fabs(error) < config.deadband)
    {
        integral = 0.0f;
        prev_error = 0.0f;
        prev_delta_error = 0.0f;
        prev_derivative = 0.0f;
        prev_measured_value = 0.0f; // 如果用了微分先行
        return last_output;
    }

    /*================= 算法分支处理 =================*/

    if (config.mode_ == Mode::POSITION)
    {
        /*---------- 位置式PID计算 ----------*/
        /* 比例项处理 */
        float proportional = config.kp * error;

        /* 积分项处理 */
        if (config.integral_enable)
        {
            if (my_fabs(error) <= config.integral_separation)
                integral += error;
            else
                integral += error * config.integral_separation / my_fabs(error);
        }
        else
        {
            if (!config.integral_separation || my_fabs(error) <= config.integral_separation)
            {
                // 积分累加并限幅
                integral += error;
            }
        }
        integral *= config.ki;
        integral = my_fmaxf(my_fminf(integral, config.max_integral), config.min_integral);
        /* 微分项处理 */
        if (config.differential_lead)
            delta_error = prev_measured_value - now_value; // 微分先行：使用测量值的变化率

        // 应用微分滤波 (一阶滞后滤波)
        current_derivative = config.kd * delta_error;
        current_derivative = config.alpha * prev_derivative + (1 - config.alpha) * current_derivative;

        output = proportional + integral + current_derivative;
    }
    else
    {
        /*---------- 增量式PID核心计算 ----------*/
        // 微分项处理
        current_derivative = config.kd * (delta_error - prev_delta_error);

        // 计算增量
        float delta_output = config.kp * delta_error +
                             config.ki * error +
                             current_derivative;

        // 输出增量限幅
        delta_output = my_fmaxf(my_fminf(delta_output, config.max_delta_output), config.min_delta_output);

        // 输出合成
        output = last_output + delta_output;
    }

    /*============== 后处理 ==============*/
    // 输出限幅
    output = my_fmaxf(my_fminf(output, config.max_output), config.min_output);

    /* 更新状态 */
    prev_measured_value = now_value;
    last_output = output;
    prev_error = error;
    prev_delta_error = delta_error;
    prev_derivative = current_derivative;

    return output;
}

void pid::set_integral(float min_integral, float max_integral)
{
    config.min_integral = min_integral;
    config.max_integral = max_integral;
}

void pid::set_delta_output(float min_delta_output, float max_delta_output)
{
    config.min_delta_output = min_delta_output;
    config.max_delta_output = max_delta_output;
}
void pid::set_output(float min_output, float max_output)
{
    config.min_output = min_output;
    config.max_output = max_output;
}

void pid::reset()
{
    integral = 0;
    prev_error = 0;
    prev_delta_error = 0;
    prev_measured_value = 0;
    prev_derivative = 0;
    last_output = 0;
}

// 在应用PID前进行死区补偿
bool pid::apply_deadzone(float target_speed, float deadband)
{
    if (my_fabs(target_speed) < deadband)
    {
        return false; // 目标速度在死区内时直接返回0
    }
    return true;
}
inline float pid::my_fabs(float x)
{
    return (x >= 0) ? x : -x;
}

inline float pid::my_fminf(float a, float b)
{
    return (a < b) ? a : b;
}

inline float pid::my_fmaxf(float a, float b)
{
    return (a > b) ? a : b;
}
