#include "john.h"

JOHNTURN Control_john;

void Tread_Init()
{
    sp.set_duty(SERVO_MID_PLUS_ns);
    sp_duty = SERVO_MID_PLUS_ns;
    sp_pid.config.kp = 1.05;
}

TrackKind Choose_Kind(JOHNTURN *Control)
{
    std::vector<double> data(55);
    for (int i = 10; i < 65; ++i)
    {
        data[i - 10] = static_cast<double>(center[i]);
    }
    std::vector<double> filtered_data = first_order_filter(data, 0.2); // 一阶低通滤波
    auto [rss_linear, slope] = calc_rss_linear(filtered_data);         // 一阶
    double rss_quad = calc_rss_quadratic(filtered_data);               // 二阶
    // 判断赛道类型
    if (((fabs(slope) < 0.3 && rss_linear < 1000) && wan_flag == 0 && white_num_col_max > 62) ||
        (wan_flag && fabs(slope) < 0.15 && rss_linear < 50 && white_num_col_max > 62))
    {
        if (wan_flag)
            line_num++;
        if (line_num >= 8)
        {
            line_num = 0;
            wan_flag = 0;
        }
        Control->Track_kind = LINE;
        std::cout << "line" << std::endl;
    }
    else
    {
        if (wan_flag == 0)
            wan_num++;
        if (wan_num >= 5)
        {
            wan_flag = 1;
            wan_num = 0;
        }
        Control->Track_kind = WAN;
        std::cout << "quxian" << std::endl;
    }
}

double Get_Turn(uint32_t sp_duty)
{
    double angle = ((int32_t)sp_duty - SERVO_MID_PLUS_ns) / 5000;
    double Rad = DEGTORAD(angle);
    double K_Turn = FastTan(Rad) * 160 / 2 / 200;

    K_Turn = MAX_OUTPUT_LIMIT(K_Turn, 5);
    K_Turn = MIN_OUTPUT_LIMIT(K_Turn, -5);
    Control_john.K_Turn_ = K_Turn;
    return K_Turn;
}

void Get_Error(JOHNTURN *Control)
{
    Control->servo_turn = Point_Weight() - 80;
}

void Get_Kp(JOHNTURN *Control)
{
    if (Control->Track_kind == LINE)
    {
        if (fabs(Control->servo_turn) < 5)
            sp_pid.config.kp = 0.2;
        else
            sp_pid.config.kp = 0.4 * (1 + Control->para);
    }
    else
    {
        // sp_pid.set_kd(0.01);
        if (fabs(Control->servo_turn) < 1.5)
            sp_pid.config.kp = 0.2;
        // else if (fabs(servo_turn) < 20)
        //     sp_pid.set_kp(0.3);
        // else if (fabs(servo_turn) < 30)
        //     sp_pid.set_kp(0.8);
        else
            sp_pid.config.kp = 0.52 * (1 + Control->para);
    }
}

void Get_Speed(int chujie, JOHNTURN *Control)
{
    if (chujie || car_flag == 5)
    {
        l_target = 0;
        r_target = 0;
    }
    else
    {
        if (Control->Track_kind == WAN) // 弯道
        {
            // buzzer.setValue(0);
            l_target = 11 * (1 - Control->K_Turn_);
            r_target = 11 * (1 + Control->K_Turn_);
        }
        else
        {
            // buzzer.setValue(1);
            l_target = 16;
            r_target = 16;
        }
    }
}

void Get_Sp_Duty(JOHNTURN *Control)
{
    Control->para = ((70 - white_num_col_max) * 1.2 + fabs(white_num_col_line - 80) * 0.8) / 150;
    // 计算赛道误差
    Get_Error(Control);
    Get_Kp(Control);
    sp_duty = SERVO_MID_PLUS_ns + sp_pid.get(0, Control->servo_turn * 4000);
}