// FuzzyPID.cpp
#include "FuzzyPID.h"
#include <cmath>

FuzzyPID::FuzzyPID(Mode mode, float kp, float ki, float kd,
                   float max_out, float max_iout)
    : mode_(mode),
      kp_(kp),
      ki_(ki),
      kd_(kd),
      max_out_(max_out),
      min_out_(-max_out),
      max_iout_(max_iout),
      dead_zone_(0.0),
      derivative_filter_(0.1),
      last_error_(0),
      prev_error_(0),
      integral_(0),
      last_derivative_(0) {}

float FuzzyPID::calculate(float target, float feedback)
{
    float error = target - feedback;

    // 死区处理
    if (fabs(error) <= dead_zone_)
    {
        error = 0;
    }

    // 模糊规则调整参数
    fuzzyAdjust();

    // 积分项计算（带抗饱和）
    integral_ += error;
    if (integral_ > max_iout_)
        integral_ = max_iout_;
    else if (integral_ < -max_iout_)
        integral_ = -max_iout_;

    // 微分项计算（带滤波）
    float derivative = (error - last_error_);
    derivative = derivative_filter_ * derivative +
                 (1 - derivative_filter_) * last_derivative_;
    last_derivative_ = derivative;

    float output = 0;
    if (mode_ == POSITION)
    {
        // 位置式PID
        output = kp_ * error + ki_ * integral_ + kd_ * derivative;
    }
    else
    {
        // 增量式PID
        float delta = kp_ * (error - last_error_) +
                      ki_ * error +
                      kd_ * (error - 2 * last_error_ + prev_error_);
        output = delta;
        prev_error_ = last_error_;
    }

    // 更新历史误差
    last_error_ = error;

    // 输出限幅
    if (output > max_out_)
        output = max_out_;
    else if (output < min_out_)
        output = min_out_;

    return output;
}

void FuzzyPID::fuzzyAdjust()
{
    // 这里可以添加模糊逻辑规则
    // 示例：根据误差大小动态调整参数
    float abs_error = fabs(last_error_);
    if (abs_error > 10)
    {
        kp_ *= 1.2;
        ki_ *= 0.8;
    }
    else if (abs_error > 5)
    {
        kp_ *= 1.1;
        ki_ *= 0.9;
    }
}

// 其他成员函数实现...
