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

/*
 * 功能: 构造函数重载1
 * 参数:
 *   pwmchip_number - pwmchip编号
 *   pwmchip_pwm_number - pwm编号
 *   period_ns - PWM周期(纳秒)
 *   duty_cycle_ns - PWM占空比(纳秒)
 *   period - PWM周期(纳秒)
 * 返回: 无
 */
pwm_ctrl::pwm_ctrl(uint8_t pwmchip_number, uint8_t pwmchip_pwm_number, uint32_t period_ns, uint32_t duty_cycle_ns)
    : pwmchip_number(pwmchip_number), pwmchip_pwm_number(pwmchip_pwm_number),
      period_ns(period_ns), duty_cycle_ns(duty_cycle_ns)
{
    this->pwm_name = std::to_string(pwmchip_number) + "," + std::to_string(pwmchip_pwm_number);
    export_pwm();
    set_pwm_property("period", period_ns);
    set_pwm_property("duty_cycle", duty_cycle_ns);
    enable_or_disable(1);
    std::cout << "PWM：" << pwm_name << " init success" << std::endl;
}

/**
 * 功能: 构造函数重载2
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
    set_pwm_property("period", period_ns);
    set_pwm_property("duty_cycle", duty_cycle_ns);
    enable_or_disable(1);
    std::cout << "PWM：" << pwm_name << " init success" << std::endl;
}

/**
 * 功能: 析构函数
 * 参数: 无
 * 返回: 无
 */
pwm_ctrl::~pwm_ctrl()
{
    std::string path = PWM_CHIP_PATH + std::to_string(pwmchip_number) + "/unexport";
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        ofs << static_cast<char>('0' + pwmchip_pwm_number);
        ofs.close();
    }
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
    set_pwm_property("period", period_ns);
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
    set_pwm_property("duty_cycle", duty_cycle_ns);
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
        std::cerr << "PWM：" << pwm_name << " failed to enable or disable!!!" << std::endl;
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
 * 功能: 设置PWM属性
 * 参数:
 *   property - 属性
 *   value - 值
 * 返回: 无
 */
void pwm_ctrl::set_pwm_property(const std::string &property, uint32_t value)
{
    std::string path = get_pwm_path(property);
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        ofs << value << "\n";
        ofs.close();
    }
    else
    {
        std::cerr << "PWM：" << pwm_name << " failed to set property" << property << " to " << value << "!!!" << std::endl;
    }
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
        std::cerr << "PWM：" << pwm_name << " failed to export!!!" << std::endl;
    }
}
