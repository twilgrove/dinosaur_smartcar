/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@��   д������Ƽ�
@��   �䣺chiusir@163.com
@����IDE��Linux ������VSCode_1.93 �����ϰ汾��Cmake_3.16 �����ϰ汾
@ʹ��ƽ̨����о2K0300�þ��ɺͱ����������ܿƼ���о�þ�����չ��
@�����Ϣ�ο����е�ַ
    ��      վ��http://www.lqist.cn
    �� �� �� �̣�http://longqiu.taobao.com
    ����������Ƶ��https://space.bilibili.com/95313236
@����汾��V1.0 ��Ȩ���У���λʹ��������ϵ��Ȩ
@�ο���Ŀ���ӣ�https://github.com/AirFortressIlikara/ls2k0300_peripheral_library

@�޸����ڣ�2025-02-26
@�޸����ݣ�
@ע�����ע��鿴·�����޸�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "LQ_HW_PWM.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�HWPwm::HWPwm()
 * @����˵����Ӳ������ PWM ���޲ι��캯��
 * @����˵������
 * @�������أ���
 * @���÷�����HWPwm MyPwm;
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HWPwm::HWPwm()
{

}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�HWPwm::HWPwm(uint8_t gpio, uint8_t chNum, uint8_t pol, uint32_t period, uint32_t duty_cycle, int mux) :
                    Gpio(gpio), ChNum(chNum), Pol(pol), Period(period), Duty_cycle(duty_cycle)
 * @����˵����Ӳ������ PWM ���вι��캯��
 * @����˵����gpio        :  ��Ӧ GPIO ���ź�
 * @����˵����chNum       :  ���Ŷ�Ӧ�� PWM ͨ����(��Ӧ��ֵ���� .hpp �ļ��в鿴)
 * @����˵����pol         :  PWM ����
 * @����˵����period      :  PWM ����
 * @����˵����duty_cycle  :  PWM ռ�ձ�
 * @����˵����mux         :  GPIO ����ֵ(Ĭ��Ϊ 0b01)
 * @�������أ���
 * @���÷�����HWPwm MyPwm(64, LS_PWM0, LS_PWM_INVERSED, 10000, 1000);
 * @��ע˵����������������ʱ������ֵ�Ƕ��٣����ھ��Ƕ���
 *           ���磺Ҫ��������Ϊ50Hz������ֵ�� 50 ����
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HWPwm::HWPwm(uint8_t gpio, uint8_t chNum, uint8_t pol, uint32_t period, uint32_t duty_cycle, int mux) :
        Gpio(gpio), ChNum(chNum), Pol(pol), Period(period), Duty_cycle(duty_cycle)
{
    // ���� GPIO ����
    GpioReuse(this->Gpio, mux);
    // ��ȡ PWMx ��������ַ
    this->PWM_ADDR = LS_PWM_BASE_ADDR + this->ChNum * LS_PWM_OFFSET;
    // ��ȡ�����ַ
    this->Low_buffer  = AddressMap(this->PWM_ADDR + LS_PWM_LOW_BUFFER_OFFSET, PAGE_SIZE);
    this->Full_buffer = AddressMap(this->PWM_ADDR + LS_PWM_FULL_BUFFER_OFFSET, PAGE_SIZE);
    this->CTRL        = AddressMap(this->PWM_ADDR + LS_PWM_CTRL_OFFSET, PAGE_SIZE);
    // ��ʼ��
    this->Init();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�HWPwm::Init()
 * @����˵������ʼ������
 * @����˵������
 * @�������أ���
 * @���÷�����MyPwm.Init();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::Init()
{
    // ��������
    this->SetPeriod(this->Period);
    // ����ռ�ձ�
    this->SetDutyCycle(this->Duty_cycle);
    // ʧ��
    LS_writel(this->CTRL, 0);
    // ���� PWM ����
    this->SetPolarity(this->Pol);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�HWPwm::~HWPwm()
 * @����˵����Ӳ������ PWM ����������
 * @����˵������
 * @�������أ���
 * @���÷����������Ķ����������ڽ�����ϵͳ�Զ�����
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HWPwm::~HWPwm()
{
    munmap(this->Low_buffer, PAGE_SIZE);
    munmap(this->Full_buffer, PAGE_SIZE);
    munmap(this->CTRL, PAGE_SIZE);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void HWPwm::SetPolarity(uint8_t pol)
 * @����˵�������� PWM ����
 * @����˵����pol  :  PWM ����(��Ӧ����� .hpp �ļ��鿴)
 * @�������أ���
 * @���÷�����MyPwm.SetPolarity(LS_PWM_INVERSED);
 * @��ע˵������
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
 * @�������ƣ�void HWPwm::SetPeriod(uint32_t period)
 * @����˵�������� PWM ����
 * @����˵����period  :  PWM ����ֵ
 * @�������أ���
 * @���÷�����MyPwm.SetPeriod(10000);
 * @��ע˵��������Ҫ��ռ�ձ�ǰ����
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::SetPeriod(uint32_t period)
{
    // ����ֵ�����Դ��� PWM ������ʱ�������Ҳ�����С�� 0
    if (period > PWM_CLK_FRE || period < 0)
    {
        cerr << "Period Error" << endl;
        return;
    }
    // ������������ǰ����ȹر�ʹ��λ
    this->Disable();
    this->Period = period;
    uint_ll val = PWM_CLK_FRE / this->Period;
    LS_writel(this->Full_buffer, val);
    // ���������ں��ٴ�ʹ��
    this->Enable();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void HWPwm::SetPeriod(uint32_t period)
 * @����˵�������� PWM ռ�ձ�
 * @����˵����duty_cycle  :  PWM ռ�ձ�ֵ
 * @�������أ���
 * @���÷�����MyPwm.SetDutyCycle(1000);
 * @��ע˵��������ռ�ձ���ע�ⲻҪ��������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::SetDutyCycle(uint32_t duty_cycle)
{
    // ռ�ձȲ����Դ��������Ҳ�С����
    if (duty_cycle > this->Period || duty_cycle < 0)
    {
        cerr << "Duty_cycle Error" << endl;
        return;
    }
    // ��������ռ�ձ�ǰ����ȹر�ʹ��λ
    this->Disable();
    this->Duty_cycle = duty_cycle;
    uint_ll val = PWM_CLK_FRE / this->Period * this->Duty_cycle / this->Period;
    LS_writel(this->Low_buffer, val);
    // ���������ں��ٴ�ʹ��
    this->Enable();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void HWPwm::Enable()
 * @����˵����ʹ��
 * @����˵������
 * @�������أ���
 * @���÷�����MyPwm.Enable();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::Enable()
{
    LS_writel(this->CTRL, LS_readl(this->CTRL) | LS_PWM_CTRL_EN);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void HWPwm::Disable()
 * @����˵����ʧ��
 * @����˵������
 * @�������أ���
 * @���÷�����MyPwm.Disable();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWPwm::Disable()
{
    LS_writel(this->CTRL, LS_readl(this->CTRL) & ~LS_PWM_CTRL_EN);
}
