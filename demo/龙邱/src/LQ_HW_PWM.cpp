/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编   写：龙邱科技
@邮   箱：chiusir@163.com
@编译IDE：Linux 环境、VSCode_1.93 及以上版本、Cmake_3.16 及以上版本
@使用平台：龙芯2K0300久久派和北京龙邱智能科技龙芯久久派拓展板
@相关信息参考下列地址
    网      站：http://www.lqist.cn
    淘 宝 店 铺：http://longqiu.taobao.com
    程序配套视频：https://space.bilibili.com/95313236
@软件版本：V1.0 版权所有，单位使用请先联系授权
@参考项目链接：https://github.com/AirFortressIlikara/ls2k0300_peripheral_library

@修改日期：2025-02-26
@修改内容：
@注意事项：注意查看路径的修改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "LQ_HW_PWM.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HWPwm::HWPwm()
 * @功能说明：硬件配置 PWM 的无参构造函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：HWPwm MyPwm;
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HWPwm::HWPwm()
{

}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HWPwm::HWPwm(uint8_t gpio, uint8_t chNum, uint8_t pol, uint32_t period, uint32_t duty_cycle, int mux) :
                    Gpio(gpio), ChNum(chNum), Pol(pol), Period(period), Duty_cycle(duty_cycle)
 * @功能说明：硬件配置 PWM 的有参构造函数
 * @参数说明：gpio        :  对应 GPIO 引脚号
 * @参数说明：chNum       :  引脚对应的 PWM 通道号(对应宏值可在 .hpp 文件中查看)
 * @参数说明：pol         :  PWM 极性
 * @参数说明：period      :  PWM 周期
 * @参数说明：duty_cycle  :  PWM 占空比
 * @参数说明：mux         :  GPIO 复用值(默认为 0b01)
 * @函数返回：无
 * @调用方法：HWPwm MyPwm(64, LS_PWM0, LS_PWM_INVERSED, 10000, 1000);
 * @备注说明：该类设置周期时，输入值是多少，周期就是多少
 *           例如：要设置周期为50Hz，周期值给 50 即可
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HWPwm::HWPwm(uint8_t gpio, uint8_t chNum, uint8_t pol, uint32_t period, uint32_t duty_cycle, int mux) :
        Gpio(gpio), ChNum(chNum), Pol(pol), Period(period), Duty_cycle(duty_cycle)
{
    // 配置 GPIO 复用
    GpioReuse(this->Gpio, mux);
    // 获取 PWMx 控制器地址
    this->PWM_ADDR = LS_PWM_BASE_ADDR + this->ChNum * LS_PWM_OFFSET;
    // 获取虚拟地址
    this->Low_buffer  = AddressMap(this->PWM_ADDR + LS_PWM_LOW_BUFFER_OFFSET, PAGE_SIZE);
    this->Full_buffer = AddressMap(this->PWM_ADDR + LS_PWM_FULL_BUFFER_OFFSET, PAGE_SIZE);
    this->CTRL        = AddressMap(this->PWM_ADDR + LS_PWM_CTRL_OFFSET, PAGE_SIZE);
    // 初始化
    this->Init();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HWPwm::Init()
 * @功能说明：初始化函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：MyPwm.Init();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::Init()
{
    // 设置周期
    this->SetPeriod(this->Period);
    // 设置占空比
    this->SetDutyCycle(this->Duty_cycle);
    // 失能
    LS_writel(this->CTRL, 0);
    // 设置 PWM 极性
    this->SetPolarity(this->Pol);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HWPwm::~HWPwm()
 * @功能说明：硬件配置 PWM 的析构函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：创建的对象生命周期结束后系统自动调用
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HWPwm::~HWPwm()
{
    munmap(this->Low_buffer, PAGE_SIZE);
    munmap(this->Full_buffer, PAGE_SIZE);
    munmap(this->CTRL, PAGE_SIZE);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HWPwm::SetPolarity(uint8_t pol)
 * @功能说明：设置 PWM 极性
 * @参数说明：pol  :  PWM 极性(对应宏可在 .hpp 文件查看)
 * @函数返回：无
 * @调用方法：MyPwm.SetPolarity(LS_PWM_INVERSED);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::SetPolarity(uint8_t pol)
{
    this->Pol = pol;
    switch(this->Pol)
    {
        case LS_PWM_NORMAL:
            LS_writel(this->CTRL, LS_readl(this->CTRL) & ~LS_PWM_CTRL_INVERT);
            break;
        case LS_PWM_INVERSED:
            LS_writel(this->CTRL, LS_readl(this->CTRL) | LS_PWM_CTRL_INVERT);
            break;
        default:
            break;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HWPwm::SetPeriod(uint32_t period)
 * @功能说明：设置 PWM 周期
 * @参数说明：period  :  PWM 周期值
 * @函数返回：无
 * @调用方法：MyPwm.SetPeriod(10000);
 * @备注说明：周期要在占空比前设置
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::SetPeriod(uint32_t period)
{
    // 周期值不可以大于 PWM 控制器时钟周期且不可以小于 0
    if (period > PWM_CLK_FRE || period < 0)
    {
        cerr << "Period Error" << endl;
        return;
    }
    // 重新设置周期前最好先关闭使能位
    this->Disable();
    this->Period = period;
    uint_ll val = PWM_CLK_FRE / this->Period;
    LS_writel(this->Full_buffer, val);
    // 设置完周期后再打开使能
    this->Enable();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HWPwm::SetPeriod(uint32_t period)
 * @功能说明：设置 PWM 占空比
 * @参数说明：duty_cycle  :  PWM 占空比值
 * @函数返回：无
 * @调用方法：MyPwm.SetDutyCycle(1000);
 * @备注说明：设置占空比请注意不要大于周期
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::SetDutyCycle(uint32_t duty_cycle)
{
    // 占空比不可以大于周期且不小于零
    if (duty_cycle > this->Period || duty_cycle < 0)
    {
        cerr << "Duty_cycle Error" << endl;
        return;
    }
    // 重新设置占空比前最好先关闭使能位
    this->Disable();
    this->Duty_cycle = duty_cycle;
    uint_ll val = PWM_CLK_FRE / this->Period * this->Duty_cycle / this->Period;
    LS_writel(this->Low_buffer, val);
    // 设置完周期后再打开使能
    this->Enable();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HWPwm::Enable()
 * @功能说明：使能
 * @参数说明：无
 * @函数返回：无
 * @调用方法：MyPwm.Enable();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::Enable()
{
    LS_writel(this->CTRL, LS_readl(this->CTRL) | LS_PWM_CTRL_EN);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HWPwm::Disable()
 * @功能说明：失能
 * @参数说明：无
 * @函数返回：无
 * @调用方法：MyPwm.Disable();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::Disable()
{
    LS_writel(this->CTRL, LS_readl(this->CTRL) & ~LS_PWM_CTRL_EN);
}
