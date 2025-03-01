/*
 * 文件名: pwm_ctrl.cpp
 * 日期: 2025.02.24
 * 作者: T.Grove
 * 描述: PWM控制模块实现，通过sysfs接口控制Linux系统PWM输出
 * 版本: v1.0.0
 * 修改:
 *   - v1.0.0 (2025.02.24): 初始版本
 */

#include "pwm_ctrl.h"

/**
 * 功能: 构造函数
 * 参数:
 *   pwmchip_number - pwmchip编号
 *   pwmchip_pwm_number - pwm编号
 *   period_ns - PWM周期(纳秒)
 *   duty_cycle_ns - PWM占空比(纳秒)
 *   pwm_name - PWM名称
 * 返回: 无
 */
pwm_ctrl::pwm_ctrl(uint8_t pwmchip_number, uint8_t pwmchip_pwm_number, uint32_t period_ns, uint32_t duty_cycle_ns, std::string pwm_name)
    : pwmchip_number(pwmchip_number), pwmchip_pwm_number(pwmchip_pwm_number),
      period_ns(period_ns), duty_cycle_ns(duty_cycle_ns), pwm_name(pwm_name)
{
    export_pwm();
    set_period(period_ns);
    fd = open(get_pwm_path("duty_cycle").c_str(), O_WRONLY);
    set_duty_cycle(duty_cycle_ns);
    enable_or_disable(1);

    std::cout << "\33[34mPWM:\33[0m" << pwm_name << " init success" << std::endl;
}

/**
 * 功能: 析构函数
 * 参数: 无
 * 返回: 无
 */
pwm_ctrl::~pwm_ctrl()
{
    enable_or_disable(0);
    std::string path = PWM_CHIP_PATH + std::to_string(pwmchip_number) + "/unexport";
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        ofs << static_cast<char>('0' + pwmchip_pwm_number);
        ofs.close();
    }
    close(fd);
    std::cout << "\33[34mPWM:\33[0m" << pwm_name << " close success" << std::endl;
}

/**
 * 功能: 设置PWM周期
 * 参数:
 *   period_ns - PWM周期(纳秒)
 * 返回: 无
 */
void pwm_ctrl::set_period(uint32_t period_ns)
{
    this->period_ns = period_ns;

    std::string path = get_pwm_path("period");
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        ofs << period_ns << "\n";
        ofs.close();
    }
    else
    {
        throw std::runtime_error("Failed to set property period to " + std::to_string(period_ns));
    }
}

/**
 * 功能: 设置PWM占空比
 * 参数:
 *   duty_cycle_ns - PWM占空比(纳秒)
 * 返回: 无
 */
void pwm_ctrl::set_duty_cycle(uint32_t duty_cycle_ns)
{
    this->duty_cycle_ns = duty_cycle_ns;
    if (write(fd, std::to_string(duty_cycle_ns).c_str(), std::to_string(duty_cycle_ns).size()) == -1)
    {
        throw std::runtime_error("Failed to set property duty_cycle to " + std::to_string(duty_cycle_ns));
    }
}

/**
 * 功能: 使能或禁用PWM输出
 * 参数:
 *   enable - 使能或禁用（0：禁用，1：使能）
 * 返回: 无
 */
void pwm_ctrl::enable_or_disable(uint8_t enable)
{
    std::string path = get_pwm_path("enable");
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        ofs << static_cast<char>('0' + enable);
        ofs.close();
    }
    else
    {
        throw std::runtime_error("Failed to set property enable to " + std::to_string(enable));
    }
}

/**
 * 功能: 获取PWM路径
 * 参数:
 *   property - 属性
 * 返回: 路径
 */
std::string pwm_ctrl::get_pwm_path(const std::string &property)
{
    std::ostringstream path;
    path << PWM_CHIP_PATH << static_cast<int>(pwmchip_number) << "/pwm" << static_cast<int>(pwmchip_pwm_number) << "/" << property;
    return path.str();
}

/**
 * 功能: 导出PWM通道
 * 参数: 无
 * 返回: 无
 */
void pwm_ctrl::export_pwm()
{
    std::string path = PWM_CHIP_PATH + std::to_string(pwmchip_number) + "/export";
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        ofs << static_cast<char>('0' + pwmchip_pwm_number);
        ofs.close();
    }
    else
    {
        throw std::runtime_error("Failed to export PWM: " + pwm_name);
    }
}
