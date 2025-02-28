// FuzzyPID.h
#ifndef FUZZY_PID_H
#define FUZZY_PID_H

class FuzzyPID
{
public:
    // 控制器模式枚举
    enum Mode
    {
        POSITION,   // 位置式PID
        INCREMENTAL // 增量式PID
    };

    // 构造/析构函数
    FuzzyPID(Mode mode = POSITION,
             float kp = 1.0, float ki = 0.0, float kd = 0.0,
             float max_out = 1000, float max_iout = 500);
    ~FuzzyPID() = default;

    // 核心计算函数
    float calculate(float target, float feedback);

    // 参数设置接口
    void setGains(float kp, float ki, float kd);
    void setOutputLimits(float max, float min);
    void setIntegralLimit(float max_i);
    void setDeadZone(float zone);
    void setDerivativeFilter(float coef);

    // 状态重置
    void reset();

private:
    // 模糊规则更新参数
    void fuzzyAdjust();

    Mode mode_; // 控制模式

    // PID参数
    float kp_;
    float ki_;
    float kd_;

    // 限制参数
    float max_out_;           // 输出上限
    float min_out_;           // 输出下限
    float max_iout_;          // 积分上限
    float dead_zone_;         // 死区范围
    float derivative_filter_; // 微分滤波系数

    // 运行状态
    float last_error_;
    float prev_error_;
    float integral_;
    float last_derivative_;
};

#endif // FUZZY_PID_H
