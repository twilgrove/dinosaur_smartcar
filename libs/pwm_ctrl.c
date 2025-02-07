/*
 * 文件名: pwm_ctrl.c
 * 日期: 2025.02.07
 * 作者: T.Grove
 * 描述: PWM控制模块实现，通过sysfs接口控制Linux系统PWM输出
 * 版本: v1.0.0
 * 修改:
 *   - v1.0.0 (2025.02.07): 初始版本
 */

#include "pwm_ctrl.h"

/* 全局变量 */
PWM_Config pwm_config[PWM_ID_MAX] = {0}; // PWM配置数组

/*
 * 功能: 初始化指定的PWM通道
 * 参数:
 *   pwm_id - PWM通道自定义ID
 *   pwmchip_number - pwmchip编号
 *   pwmchip_pwm_number - pwm编号
 * ------------------------------------
 * 引脚    pwmchip编号          pwm编号
 * 64        0                    0
 * 65        1                    0
 * 66        2                    0
 * 67        3                    0
 * 88        4                    1
 * 89        4                    2
 * ------------------------------------
 *   period - PWM周期(纳秒)
 * 返回: 无
 */
void pwm_init(uint8_t pwm_id, uint8_t pwmchip_number, uint8_t pwmchip_pwm_number, uint32_t period)
{
    char path_name[128];
    char buffer[12];
    char value;
    int len;

    /* 导出PWM通道 */
    sprintf(path_name, "/sys/class/pwm/pwmchip%d/export", pwmchip_number);
    value = '0' + pwmchip_pwm_number;
    pwm_config[pwm_id].fd = open(path_name, O_WRONLY);
    write(pwm_config[pwm_id].fd, &value, 1);
    close(pwm_config[pwm_id].fd);
    printf("pwm_init_export: %d\n", pwm_id);

    /* 设置PWM周期 */
    sprintf(path_name, "/sys/class/pwm/pwmchip%d/pwm%d/period", pwmchip_number, pwmchip_pwm_number);
    pwm_config[pwm_id].period_ns = period;
    len = sprintf(buffer, "%u\n", period);
    pwm_config[pwm_id].fd = open(path_name, O_WRONLY);
    write(pwm_config[pwm_id].fd, buffer, len);
    close(pwm_config[pwm_id].fd);
    printf("pwm_init_period: %d\n", pwm_id);

    /* 初始化占空比 */
    sprintf(path_name, "/sys/class/pwm/pwmchip%d/pwm%d/duty_cycle", pwmchip_number, pwmchip_pwm_number);
    pwm_config[pwm_id].fd_pwm_duty = open(path_name, O_WRONLY);
    write(pwm_config[pwm_id].fd_pwm_duty, "0", 1);
    printf("pwm_init_duty_cycle: %d\n", pwm_id);

    /* 使能PWM输出 */
    sprintf(path_name, "/sys/class/pwm/pwmchip%d/pwm%d/enable", pwmchip_number, pwmchip_pwm_number);
    pwm_config[pwm_id].fd = open(path_name, O_WRONLY);
    write(pwm_config[pwm_id].fd, "1", 1);
    close(pwm_config[pwm_id].fd);
    printf("pwm_init_enable: %d\n", pwm_id);
}

/*
 * 功能: 设置PWM周期
 * 参数:
 *   pwm_id - PWM通道ID
 *   period - 新的PWM周期(纳秒)
 * 返回: 无
 */
void pwm_set_period(uint8_t pwm_id, uint32_t period)
{
    char path_name[128];
    char buffer[12];
    int len;

    sprintf(path_name, "/sys/class/pwm/pwmchip%d/pwm%d/period",
            pwm_config[pwm_id].pwmchip_number,
            pwm_config[pwm_id].pwmchip_pwm_number);
    len = sprintf(buffer, "%u\n", period);

    pwm_config[pwm_id].fd = open(path_name, O_WRONLY);
    write(pwm_config[pwm_id].fd, buffer, len);
    close(pwm_config[pwm_id].fd);
}

/*
 * 功能: 设置PWM占空比
 * 参数:
 *   pwm_id - PWM通道ID
 *   duty_cycle_percent - 占空比百分比(0-1)
 * 返回: 无
 */
void pwm_set_duty_cycle(uint8_t pwm_id, float duty_cycle_percent)
{
    char buffer[12];
    int len;

    /* 将百分比转换为纳秒值 */
    pwm_config[pwm_id].duty_cycle_ns = (uint32_t)(duty_cycle_percent * pwm_config[pwm_id].period_ns);
    len = sprintf(buffer, "%u\n", pwm_config[pwm_id].duty_cycle_ns);
    printf("pwm_set_duty_cycle: %d, %2.f\n", pwm_config[pwm_id].duty_cycle_ns, duty_cycle_percent);

    write(pwm_config[pwm_id].fd_pwm_duty, buffer, len);
    /* 注意：此处不关闭文件描述符，因为duty_cycle会频繁修改 */
}

/*
 * 功能: 使能或禁用PWM输出
 * 参数:
 *   pwm_id - PWM通道ID
 *   enable - 1:使能, 0:禁用
 * 返回: 无
 */
void pwm_enable_or_disable(uint8_t pwm_id, uint8_t enable)
{
    char path_name[128];
    char value;

    sprintf(path_name, "/sys/class/pwm/pwmchip%d/pwm%d/enable",
            pwm_config[pwm_id].pwmchip_number,
            pwm_config[pwm_id].pwmchip_pwm_number);
    value = '0' + enable;

    pwm_config[pwm_id].fd = open(path_name, O_WRONLY);
    write(pwm_config[pwm_id].fd, &value, 1);
    close(pwm_config[pwm_id].fd);
}

/*
 * 功能: 移除PWM通道配置
 * 参数:
 *   pwm_id - PWM通道ID
 * 返回: 无
 */
void pwm_remove(uint8_t pwm_id)
{
    char path_name[128];

    /* 关闭占空比文件描述符 */
    close(pwm_config[pwm_id].fd_pwm_duty);

    /* 取消导出PWM通道 */
    sprintf(path_name, "/sys/class/pwm/pwmchip%d/unexport", pwm_config[pwm_id].pwmchip_number);
    pwm_config[pwm_id].fd = open(path_name, O_WRONLY);
    write(pwm_config[pwm_id].fd, "0", 1);
    close(pwm_config[pwm_id].fd);
}