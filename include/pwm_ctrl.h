#ifndef __PWM_CTRL_H__
#define __PWM_CTRL_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#define PWM_CHIP_PATH "/sys/class/pwm/pwmchip"

/**
 * ------------------------------------
 * 引脚    pwmchip编号          pwm编号
 * 64        0                    0
 * 65        1                    0
 * 66        2                    0
 * 67        3                    0
 * 88        4                    1
 * 89        4                    2
 * ------------------------------------
 */
class pwm_ctrl
{
public:
    pwm_ctrl(uint8_t pwmchip_number, uint8_t pwmchip_pwm_number, uint32_t period_ns, uint32_t duty_cycle_ns);
    pwm_ctrl(uint8_t pwmchip_number, uint8_t pwmchip_pwm_number, uint32_t period_ns, uint32_t duty_cycle_ns, std::string pwm_name);
    ~pwm_ctrl();

    void set_period(uint32_t period_ns);         // 设置PWM周期
    void enable_or_disable(uint8_t enable);      // 使能或禁用PWM输出
    void set_duty_cycle(uint32_t duty_cycle_ns); // 设置PWM占空比
private:
    uint8_t pwmchip_number;     // pwmchip编号
    uint8_t pwmchip_pwm_number; // pwm编号
    uint32_t period_ns;         // pwm周期(纳秒)
    uint32_t duty_cycle_ns;     // pwm占空比(纳秒)
    std::string pwm_name;       // pwm名称

    std::string get_pwm_path(const std::string &property);              // 获取PWM的路径
    void set_pwm_property(const std::string &property, uint32_t value); // 设置PWM属性（如周期和占空比）
    void export_pwm();                                                  // 导出PWM通道
};

#endif // __PWM_CTRL_H__
