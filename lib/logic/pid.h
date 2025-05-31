/*
 * 文件名: pid.h
 * 日期: 2025.2.27
 * 作者: T.Grove
 * 描述: PID控制器头文件，支持位置式和增量式PID算法
 * 版本: v1.0.0
 * 修改:
 *   - v1.0.0 (2025.2.27): 初始版本，实现基础的位置式和增量式PID算法
 */
#ifndef PID_H
#define PID_H

#include <cmath>
#include <stdexcept>
/*
 *注意事项
 *    - 使用位置式PID时必须设置积分限幅参数，
 *      对于响应较快的系统（如电机控制）：积分限幅一般设置为输出限幅的 30% ~ 50%。
 *      对于响应较慢的系统（如温度控制）：积分限幅一般设置为输出限幅的 50% ~ 70%。
 *    - 所有内部状态变量必须初始化为0
 *    - 优化参数根据实际需求配置，不需要时可设置为0
 *
 * 舵机:位置式PD
 * 电机:增量式PID
 */
bool apply_deadzone(float target_speed, float deadband); // 死区补偿

class pid
{
public:
    enum class Mode
    {
        POSITION, // 位置式PID
        INCREMENT // 增量式PID
    };
    // 模式,      比例系数,    积分系数,   微分系数, (积分限幅/输出增量限幅)绝对值, pid输出限幅
    pid(Mode mode, float kp, float ki, float kd, float abs_process, float max_output, float min_output);

    ~pid() = default;

    void set_kp(float kp); // [通用] 设置比例系数
    void set_ki(float ki); // [通用] 设置积分系数
    void set_kd(float kd); // [通用] 设置微分系数

    void set_integral(float min_integral, float max_integral);             // [位置式] 设置积分限幅
    void set_delta_output(float min_delta_output, float max_delta_output); // [增量式] 设置输出增量限幅
    void set_output(float min_output, float max_output);                   // [通用] 设置输出限幅

    void set_alpha(float alpha);                              // [位置式] 设置微分滤波系数
    void set_deadband(float deadband);                        // [通用] 设置误差死区范围
    void set_integral_separation(float separation_threshold); // [位置式] 设置积分分离阈值
    void set_differential_lead(bool differential_lead);       // [位置式] 设置微分先行使能
    void reset();                                             // [通用] 重置PID控制器内部状态
    float get(float set_value, float now_value);              // 计算PID输出

    Mode mode_; // PID模式
    /* 基础参数 */
    float kp = 0; // [通用] 比例系数 - 控制系统对偏差的响应强度，不能为0
    float ki = 0; // [通用] 积分系数 - 消除稳态误差，可以为0
    float kd = 0; // [通用] 微分系数 - 抑制超调，可以为0

    /* 限幅参数 */
    float max_delta_output = 0; // [增量式] 输出增量上限 - 必须设置且大于min_delta_output
    float min_delta_output = 0; // [增量式] 输出增量下限 - 必须设置且小于max_delta_output
    float max_integral = 0;     // [位置式] 积分项上限 - 必须大于min_integral
    float min_integral = 0;     // [位置式] 积分项下限 - 必须小于max_integral
    float max_output = 0;       // [通用] PID输出上限 - 必须设置且大于min_output
    float min_output = 0;       // [通用] PID输出下限 - 必须设置且小于max_output

    /* 优化参数 */
    float alpha = 0;               // [位置式] 微分滤波系数 - 范围0-1，不使用设为0
    float deadband = 0;            // [通用] 误差死区范围 - 不使用设为0
    float integral_separation = 0; // [位置式] 积分分离阈值 - 不使用设为0
    bool differential_lead = 0;    // [位置式] 微分先行使能 - 1:启用 0:禁用，不使用设为0

    /* 内部状态 */
    float prev_error = 0;          // [通用] 上一次误差 - 初始化必须置0
    float prev_delta_error = 0;    // [通用] 上一次误差变化量 - 初始化必须置0
    float prev_measured_value = 0; // [通用] 上一次测量值 - 初始化必须置0
    float prev_derivative = 0;     // [通用] 上一次微分值 - 初始化必须置0
    float integral = 0;            // [位置式] 积分项 - 初始化必须置0
    float last_output = 0;         // [增量式] 上一次输出值 - 初始化必须置0
};
#endif // PID_H
