
#include "headfile.h"
// 定义差速模型宏及变量
#define LDISTANCE 200 // 车身的前后轮中心距 mm
#define BORDWIDTH 155 // 两后轮中心距     mm
#define A 0.8         // 差速的大小系数1.05
#define K 1.06        // 差速的预知系数  K越大差速越提前1.06
#define AK 1
unsigned int jisha_flag = 0;
float g_Control = 0, g_Control_last = 0, g_Control_pre = 0, casu_weight = 0.38; // 1.29;//原0.6
unsigned int p_chasu_quanzh = 0;
unsigned int p_chasu_sudu = 0;
unsigned int tingche_way = 1;
unsigned int ruku_zuobiao_hang_jubu = 0;
unsigned int ruku_youbiao_hang_jubu = 0;
unsigned int ruku_zuobiao_hang = 0;
unsigned int ruku_youbiao_hang = 0;
unsigned int ruku_zuobiao_lie = 0;
unsigned int ruku_youbiao_lie = 0;
unsigned int zhidao_juli = 0;
unsigned int zhidao_flag = 0;
int chasuxisu = 430; // 电机差速系数340
int yuzhi_speed = 0;
float angle = 0;
unsigned int po = 0;
int Max_Speed = 9500;
extern int left_huan_num, right_huan_num;
extern int sousuojieshuhang; // 搜索结束行
unsigned int cnt = 0;
float K1 = 0.3; // k1为加速度计计算出角度占的比例，越大越受速度影响，小了动态效果不好,改为3和7 计算速度快
float Accel_Y, Gyro_Y, Accel_X, Accel_Z;
float K2 = 0.027;
unsigned int s = 0, stop = 0, poer_flag2 = 0, poer_flag = 0, poer2 = 0;
int left_whitenum = 0, right_whitenum = 0;
int speed_yingshe = 0;
unsigned int car_gogogo = 0;
/***编码器相关变量***/
int left_num, right_num, LeftOut, RightOut, left_zheng_ma = 0, right_zheng_ma = 0, left_fan_ma, right_fan_ma;
long long int MOTOR_Duty = 0;
int S_D5_Duty, S_D5_Duty2;
int chasuzuo = 0; // 差速左
int chasuyou = 0; // 差速右
int por_cnt = 0;  // 差速右
long long int angle2 = 0;
float x, j; // 舵机打脚
long long int Target_Speed1 = 0, Target_Speed2 = 0;
float piancha; // 舵机偏差   通过摄像头中线处理得到
int Set = 7;
int lock_flag = 0;
float g_fSpeedControlOutNew = 10000;            // pid算出的速度
float g_fSpeedControlIntegral;                  // pid速度积分部分
float Real_speed = 0;                           // 现实生活中的速度
int g_nRightMotorPulseSigma;                    // 右边总脉冲
int g_nLeftMotorPulseSigma;                     // 左边总脉冲
unsigned int leijia_flag = 0, leijia_flag2 = 0; // 编码器累加标志
int left_leijia = 0, right_leijia = 0;
int left_leijia2 = 0, right_leijia2 = 0, right_leijia_por = 0, tingche_leijia_por = 0, huan2_leijia_por = 0;
int g_fCarSpeed = 0; // 小车实际速度

int zhongzhi = 99;
int tingche_flag = 0, tingche_flag2 = 0; // 停车标志
int stop_flag = 0;                       // 驱动断电标志

int wailun_speed = 0;
int julu_speed = 0;
int Set_Speed1 = 370;
int Set_Speed11 = 610; /// 550  575
int Set_Speed2 = 300;
int you_speed = 0;
int zuo_speed = 0;
int jiasu_part = 0, jiasha_part = 0;
int chujie_num = 0;
int chujie = 0;
// const unsigned int left[70]={
// 97, 97, 96, 96, 95, 95, 95, 95, 94, 94,
// 94, 93, 93, 93, 92, 92, 92, 91, 91, 91,
// 90, 90, 90, 89, 89, 88, 88, 88, 88, 87,
// 87, 87, 86, 86, 86, 85, 85, 84, 84, 84,
// 83, 83, 83, 82, 82, 82, 81, 81, 81, 80,
// 80, 79, 79, 79, 78, 78, 78, 77, 77, 77,
// 76, 76, 75, 75, 75, 74, 74, 74, 73, 73
// };
//
const unsigned int left[70] = {
    95, 95, 95, 95, 94, 94, 94, 94, 93, 93,
    92, 92, 92, 92, 91, 91, 91, 91, 90, 90,
    90, 90, 89, 89, 89, 89, 88, 88, 88, 88,
    87, 87, 87, 87, 87, 87, 86, 86, 86, 86,
    86, 85, 85, 85, 85, 85, 84, 84, 84, 84,
    83, 83, 83, 83, 82, 82, 82, 82, 81, 81,
    81, 81, 81, 80, 80, 80, 80, 79, 79, 79};
////const unsigned int right[70]={
////
////105, 105, 106, 106, 107, 107, 107, 108, 108, 108,
////109, 109, 109, 110, 110, 110, 111, 111, 111, 112,
////112, 112, 113, 113, 113, 114, 114, 114, 115, 115,
////116, 116, 116, 117, 117, 117, 118, 118, 118, 119,
////119, 120, 120, 120, 121, 121, 121, 122, 122, 122,
////123, 123, 123, 124, 124, 124, 125, 125, 125, 126,
////126, 127, 127, 127, 128, 128, 128, 129, 129, 129
////};
//
const unsigned int right[70] = {
    99, 99, 99, 99, 99, 100, 100, 101, 101, 101,
    102, 102, 102, 103, 103, 103, 103, 104, 104, 105,
    105, 105, 105, 106, 106, 106, 107, 107, 107, 108,
    108, 108, 109, 109, 109, 110, 110, 110, 110, 110,
    111, 111, 111, 111, 112, 112, 112, 112, 113, 113,
    113, 113, 114, 114, 114, 115, 115, 115, 115, 116,
    116, 116, 117, 117, 117, 118, 118, 118, 119, 119};
//
// const unsigned int left[70]={
// 97, 97, 96, 96, 95, 95, 95, 95, 94, 94,
// 94, 93, 93, 93, 92, 92, 92, 91, 91, 91,
// 90, 90, 90, 89, 89, 88, 88, 88, 88, 87,
// 87, 87, 86, 86, 86, 85, 85, 84, 84, 84,
// 83, 83, 83, 82, 82, 82, 81, 81, 81, 80,
// 80, 79, 79, 79, 78, 78, 78, 77, 77, 77,
// 76, 76, 75, 75, 75, 74, 74, 74, 73, 73
//};
// const unsigned int right[70]={
//
// 105, 105, 106, 106, 107, 107, 107, 108, 108, 108,
// 109, 109, 109, 110, 110, 110, 111, 111, 111, 112,
// 112, 112, 113, 113, 113, 114, 114, 114, 115, 115,
// 116, 116, 116, 117, 117, 117, 118, 118, 118, 119,
// 119, 120, 120, 120, 121, 121, 121, 122, 122, 122,
// 123, 123, 123, 124, 124, 124, 125, 125, 125, 126,
// 126, 127, 127, 127, 128, 128, 128, 129, 129, 129
//};

void proess() // 处理流程
{
    // Ostu();
    //  threshold1= my_adapt_threshold(mt9v03x_image[0],188,70);
    //  SignalProcess_grayfine_fill();
    //// 固定编码器值完成出库操作
    if (car_gogogo == 0)
    {

        leijia_flag2 = 1;         // 编码器读值
        if (right_leijia2 > 2100) ////right_leijia2大于6000   car_gogogo=1
        {
            // if (c_r == 2) // zuo出库
            // pwm_duty(ATOM1_CH1_P33_9, stree_max - 10);
            // else if (c_r == 1) // zuo出库
            // pwm_duty(ATOM1_CH1P33_9, stree_min + 10);
        }
        if (c_r == 2)                 // zuo出库
            dianjiqudong(6000, 8200); // 6000
        else if (c_r == 1)            // zuo出库
            dianjiqudong(8200, 6000); // 6000
    }
    ////出库完成，正式赛道
    if (car_gogogo)
    {
        // po = adc_mean_filter(ADC_0, ADC0_CH8_A8, ADC_12BIT, 2);

        if (po <= 100)
        {
            poer++;
            if (poer >= 3)
            {
                time11 = 0;
                if (por_cnt == 0 && huihuan_num > 35 && !left_huan_num && !right_huan_num && !three_cross && !three_cross1 && !lefthuihuan_flag && !lefthuihuan_flag) //||three_cross_cnt==1  por_cnt==0&&
                {
                    poer_flag = 1; ////避障标志位
                    //                     uart_putchar(WIRELESS_UART, '9');poer_flag por_cnt
                    //                     uart_putchar(WIRELESS_UART, '1');
                    //                     uart_putchar(WIRELESS_UART, ' ');
                }
            }
            if (poer > 10)
                poer = 10;
        }
        else
            poer = 0;
        if (poer_flag == 1 && po > 100)
        {
            poer_flag = 2;
            //   three_cross=0;
            star_lineflag = 0;
            right_huan_num = 0;
            left_huan_num = 0;
        }
        if (poer_flag || por_cnt)
        {
            /// three_cross=0;
            star_lineflag = 0;
            right_huan_num = 0;
            left_huan_num = 0;
        }
        if (poer_flag == 2 && po <= 100)
        {
            poer_flag = 0;
            por_cnt = 1;
            leijia_flag2 = 1;
        }

        if (star_lineflag == 0 && !poer_flag) //
        {
            check_starting_line(); //&&sousuojieshuhang<16&&zhidao_juli<110   ////检测起跑线更新star_lineflag
            if (star_lineflag == 1)
            {
                park_flag++; ////park_flag==1时正常跑，==2入库操作
                //                             uart_putchar(WIRELESS_UART, '5');//
                //                             uart_putchar(WIRELESS_UART, '5');
                //                             uart_putchar(WIRELESS_UART, ' ');
            }
            if (park_flag >= 2)
            {
                park_flag = 2;
            }
        }

        if (star_lineflag)
        {
            three_cross = 0;
            right_huan_num = 0;
            left_huan_num = 0;
        }

        Center_line_deal();
        if (tingche_flag)
        {
            if ((Right_Add_num <= 3) && (Left_Add_num <= 3) && (zuodiuxianshu <= 3) && (youdiuxianshu <= 3))
            {
                tingche_flag2 = 1;
            }
        }

        if (park_flag != 2)
        {
            you_huihuan();
            zuo_huihuan();
            youhuandao();
            zuohuandao();
        }
        speed_get();
        ////中线修复
        Mid_Line_Repair(sousuojieshuhang);
        //                    if(time5<=30)
        //                    Point= center [10]+center[12]+center[14] /3;
        //                    else
        Get_Point = Point_Weight();
        if (chujie == 0)
            S_D5_Duty = PlacePID_Control(&S_D5_PID, zhongzhi, Points);
        else
            S_D5_Duty = PlacePID_Control(&S_D5_PID, zhongzhi, 101);
        p_chasu_quanzh = abs(Points - zhongzhi) - 15;
        if (p_chasu_quanzh >= 25)
            p_chasu_quanzh = 25;
        p_chasu_quanzh = 0.4 * p_chasu_quanzh;
        p_chasu_sudu = 0.5 * (wailun_speed - 410);

        // S_D5_Duty=Turn_Ctl_fuzzy(Point);

        ////入库参数大小操作
        if (park_flag == 2)
        {
            if (c_r == 2)
                ruku_handle();
            else if (c_r == 1)
                ruku_handle2();
        }

        ////出界保护
        chujiebaohu();
        if (chujie_num > 130 && chujie == 0 && park_flag != 2)
        {
            chujie = 1;
            {
                // uart_putchar(WIRELESS_UART, 'o');//
                // uart_putchar(WIRELESS_UART, 'k');
                // uart_putchar(WIRELESS_UART, ' ');
            }
        }

        if (my_time > 1000)
        {
            my_time = 1000;
        }

        if (time1 > 1000)
        {
            time1 = 1000;
        }

        if (time11 > 1000)
        {
            time11 = 1000;
        }

        if (time2 > 1000)
        {
            time2 = 1000;
        }

        if (time22 > 1000)
        {
            time22 = 1000;
        }

        if (time3 > 1000)
        {
            time3 = 1000;
        }

        if (time4 > 1000)
        {
            time4 = 1000;
        }
        if (time5 > 2000)
        {
            time5 = 2000;
        }
    }
}
void GetMotorPulse(void) // 编码器脉冲100ms计算一次
{
    g_nRightMotorPulseSigma = right_zheng_ma - right_fan_ma;
    g_nLeftMotorPulseSigma = left_zheng_ma - left_fan_ma;
    right_zheng_ma = 0;
    right_fan_ma = 0;
    left_zheng_ma = 0;
    left_fan_ma = 0;
}

float Get_Dire(unsigned int i)
{
    g_Control = -(S_D5_Duty - stree_center);

    float tn = tan(g_Control / 54.95 * casu_weight); // 54.95=70*0.785
    if (i)                                           /// you轮
    {

        return (A * (K - 0.5 * BORDWIDTH * tn / LDISTANCE * AK));

        // return 1;
    }
    // zuo轮
    else
    {
        return (A * (K + 0.5 * BORDWIDTH * tn / LDISTANCE * AK));
    }
}

void SpeedControl(void) // 速度控制
{
    // if(right_huan_num==0&&left_huan_num==0)

    //  if(0.00192*(abs(Point-zhongzhi)-30)*(abs(Point-zhongzhi)-30)*(abs(Point-zhongzhi)-30)+chasuxisu)
    //   if(p_chasu>=35) p_chasu=35;

    if (zhidao_juli < 100)
        S_D5_Duty2 = (S_D5_Duty - stree_center) * (chasuxisu) / 100;
    else
        S_D5_Duty2 = (S_D5_Duty - stree_center) * (chasuxisu - 350) / 100;
    // if(left_huan_num||right_huan_num||lefthuihuan_flag||youhuihuan_flag||three_cross1||three_cross)
    // S_D5_Duty2=(wailun_speed/(410.0))*(S_D5_Duty-stree_center)*(chasuxisu)/100;   //x为差速系数/100p_chasu+
    // S_D5_Duty2=(S_D5_Duty-stree_center)*(0.3*p_chasu_sudu+0.7*p_chasu_quanzh+chasuxisu)/100;
    // S_D5_Duty2=60;//+0.5*p_chasu_quanzh

    /***停车部分***/

    if (c_r == 2 && tingche_flag == 1)
    {
        S_D5_Duty2 = 450;
        if (tingche_flag2 == 1)
        {
            S_D5_Duty2 = 450;
            if (right_num < 50 || left_num < 50)
            {
                S_D5_Duty2 = 0;
                Set_Speed2 = 0;
            }
        }
    }
    else if (c_r == 1 && tingche_flag == 1)
    {
        S_D5_Duty2 = -450;
        if (tingche_flag2 == 1)
        {
            S_D5_Duty2 = -450;
            if (right_num < 50 || left_num < 50)
            {
                S_D5_Duty2 = 0;
                Set_Speed2 = 0;
            }
        }
    }
    /***停车部分结束***/

    if (S_D5_Duty2 >= 0)
    {
        wailun_speed = (right_num + left_num) / 2; // Set_Speed2
    }
    else
    {
        wailun_speed = (right_num + left_num) / 2;
    }
    if (S_D5_Duty2 > 430)
        S_D5_Duty2 = 430;
    else if (S_D5_Duty2 <= -430)
        S_D5_Duty2 = -430;

    if (!(
            left_huan_num == 6 || right_huan_num == 6 //||
                                                      //      left_huan_num==7||right_huan_num==7||
                                                      //     left_huan_num==5||right_huan_num==5

            ) &&
        (lefthuihuan_flag == 1 || lefthuihuan_flag == 2 || youhuihuan_flag == 1 || youhuihuan_flag == 2))
    {

        if (wailun_speed < 360)
        {
            if (S_D5_Duty2 >= 0)
            {
                if (abs(0.1 * S_D5_Duty2) < 60)
                    you_speed = Set_Speed2 + 0.09 * S_D5_Duty2; // Set_Speed2  0.4
                else
                    you_speed = Set_Speed2 + 60;
                zuo_speed = Set_Speed2 - S_D5_Duty2; //
            }
            else
            {
                if (abs(0.1 * S_D5_Duty2) < 60)
                    zuo_speed = Set_Speed2 - 0.09 * S_D5_Duty2;
                else
                    zuo_speed = Set_Speed2 + 60;
                you_speed = Set_Speed2 + S_D5_Duty2; //
            }
        }
        else
        {

            if (S_D5_Duty2 >= 0)
            {
                you_speed = Set_Speed2 + 0 * S_D5_Duty2; // Set_Speed2  太公17号是 0.1
                zuo_speed = Set_Speed2 - S_D5_Duty2;     //
            }
            else
            {
                zuo_speed = Set_Speed2 - 0 * S_D5_Duty2;
                you_speed = Set_Speed2 + S_D5_Duty2; //
            }
        }
    }
    else if ((
                 left_huan_num == 6 || right_huan_num == 6
                 //  left_huan_num==7||right_huan_num==7||
                 //   left_huan_num==5||right_huan_num==5
                 ) &&
             !(lefthuihuan_flag == 1 || lefthuihuan_flag == 2 || youhuihuan_flag == 1 || youhuihuan_flag == 2))
    {

        if (S_D5_Duty2 >= 0)
        {
            if (abs(0.32 * S_D5_Duty2) < 80)
                you_speed = Set_Speed2 + 0.32 * S_D5_Duty2; // Set_Speed2  0.4
            else
                you_speed = Set_Speed2 + 80;
            zuo_speed = Set_Speed2 - S_D5_Duty2; //
        }
        else
        {
            if (abs(0.32 * S_D5_Duty2) < 80)
                zuo_speed = Set_Speed2 - 0.32 * S_D5_Duty2;
            else
                zuo_speed = Set_Speed2 + 80;
            you_speed = Set_Speed2 + S_D5_Duty2; //
        }
    }
    else
    {

        if (wailun_speed < 360 && !left_huan_num && !right_huan_num)
        {
            if (S_D5_Duty2 >= 0)
            {
                if (abs(0.05 * S_D5_Duty2) < 35)                // 0.08
                    you_speed = Set_Speed2 + 0.05 * S_D5_Duty2; // Set_Speed2  0.4
                else
                    you_speed = Set_Speed2 + 35;     // 60
                zuo_speed = Set_Speed2 - S_D5_Duty2; //
            }
            else
            {
                if (abs(0.05 * S_D5_Duty2) < 35)
                    zuo_speed = Set_Speed2 - 0.05 * S_D5_Duty2;
                else
                    zuo_speed = Set_Speed2 + 35;
                you_speed = Set_Speed2 + S_D5_Duty2; //
            }
        }
        else
        {

            if (S_D5_Duty2 >= 0)
            {
                you_speed = Set_Speed2 + 0 * S_D5_Duty2; // Set_Speed2  太公17号是 0.1
                zuo_speed = Set_Speed2 - S_D5_Duty2;     //
            }
            else
            {
                zuo_speed = Set_Speed2 - 0 * S_D5_Duty2;
                you_speed = Set_Speed2 + S_D5_Duty2; //
            }
        }
    }

    //                 you_speed=Set_Speed2*Get_Dire(1);//Set_Speed2  0.3
    //                 zuo_speed=Set_Speed2*Get_Dire(0);//

    /***停车部分***/

    if (c_r == 2 && tingche_flag == 1)
    {
        S_D5_Duty2 = 450;
        if (tingche_flag2 == 1)
        {
            S_D5_Duty2 = 450;
            if (right_num < 50 || left_num < 50)
            {
                S_D5_Duty2 = 0;
                Set_Speed2 = 0;
            }
        }
        if (S_D5_Duty2 >= 0)
        {
            you_speed = Set_Speed2 + 0 * S_D5_Duty2; // Set_Speed2  太公17号是 0.1
            zuo_speed = Set_Speed2 - S_D5_Duty2;     //
        }
        else
        {
            zuo_speed = Set_Speed2 - 0 * S_D5_Duty2;
            you_speed = Set_Speed2 + S_D5_Duty2; //
        }
    }
    else if (c_r == 1 && tingche_flag == 1)
    {
        S_D5_Duty2 = -450;
        if (tingche_flag2 == 1)
        {
            S_D5_Duty2 = -450;
            if (right_num < 50 || left_num < 50)
            {
                S_D5_Duty2 = 0;
                Set_Speed2 = 0;
            }
        }
        if (S_D5_Duty2 >= 0)
        {
            you_speed = Set_Speed2 + 0 * S_D5_Duty2; // Set_Speed2  太公17号是 0.1
            zuo_speed = Set_Speed2 - S_D5_Duty2;     //
        }
        else
        {
            zuo_speed = Set_Speed2 - 0 * S_D5_Duty2;
            you_speed = Set_Speed2 + S_D5_Duty2; //
        }
    }
    /***停车部分结束***/

    Target_Speed1 = PID_Cascade(&MOTOR_PID, left_num, zuo_speed);    // speed_setSet_Speed2//S_D5_Duty2
    Target_Speed2 = PID_Cascade2(&MOTOR2_PID, right_num, you_speed); // S_D5_DutyS_D5_Duty2//S_D5_Duty2
    // Target_Speed1+=PID_Realize(&MOTOR_PID,(left_num+right_num)/2, Set_Speed2);

    if (Target_Speed1 > Max_Speed)
        Target_Speed1 = Max_Speed;
    else if (Target_Speed1 < -Max_Speed)
        Target_Speed1 = -Max_Speed;

    if (Target_Speed2 > Max_Speed)
        Target_Speed2 = Max_Speed;
    else if (Target_Speed2 < -Max_Speed)
        Target_Speed2 = -Max_Speed;

    dianjiqudong(Target_Speed1, Target_Speed2);
}

void chasu(float x) // 过弯差速    去学校更差速系数chasuxisu  越小差速越明显 最小为1
{
    if (x > 0)
    {
        chasuzuo = MOTOR_Duty * x / chasuxisu;        //-
        chasuyou = -0.9 * MOTOR_Duty * x / chasuxisu; //+
    }
    else
    {
        chasuyou = MOTOR_Duty * (-x) / chasuxisu;        ///////+
        chasuzuo = -0.9 * MOTOR_Duty * (-x) / chasuxisu; //////-
    }
}

void dianjiqudong(int speed1, int speed2) // 电机驱动函数     去学校修改正反和y1  y2
{
    // if (speed1 >= 0)
    // {
    //     gpio_set(P21_2, 1);
    //     pwm_duty(ATOM0_CH1_P21_3, speed1 + 350);
    // }
    // else if (speed1 < 0)
    // {
    //     gpio_set(P21_2, 0);
    //     pwm_duty(ATOM0_CH1_P21_3, abs(speed1 - 350));
    // }
    // if (speed2 >= 0)
    // {

    //     gpio_set(P21_5, 1);
    //     pwm_duty(ATOM0_CH2_P21_4, speed2 + 300);
    // }
    // else if (speed2 < 0)
    // {
    //     gpio_set(P21_5, 0);
    //     pwm_duty(ATOM0_CH2_P21_4, abs(speed2 - 300));
    // }
}
void read_1Ms_pulse(void) // 脉冲数读取10ms    去学校测试几ms合适
{
    // right_num = gpt12_get(GPT12_T6); // 保存脉冲计数器计算值右

    // left_num = -gpt12_get(GPT12_T4);
    // ////均速小于阈值就加速
    // if (wailun_speed < yuzhi_speed)
    // {
    //     jisha_flag = 0;
    // }

    // if (leijia_flag2 == 1) // 和倒库时计算距离////根据右轮编码器确定发车和时间戳
    // {

    //     right_leijia2 += right_num;
    //     if (right_leijia2 > 6000)
    //     {
    //         car_gogogo = 1;
    //         time = 0;
    //     }
    // }
    // if (por_cnt)
    // {
    //     if (c_r == 2) // zuo出库
    //         right_leijia_por += right_num;
    //     else if (c_r == 1) // zuo出库
    //         right_leijia_por += left_num;

    //     if (right_leijia_por > 7500)
    //     {
    //         por_cnt = 0;
    //         right_leijia_por = 0;
    //         leijia_flag2 = 0;
    //     }
    // }
    // else
    //     right_leijia_por = 0;

    // if (star_lineflag)
    // {
    //     tingche_leijia_por += right_num;
    //     if (tingche_leijia_por > 11000)
    //     {
    //         star_lineflag = 0;
    //         tingche_leijia_por = 0;
    //     }
    // }
    // else
    //     tingche_leijia_por = 0;

    // if (huan2_flag)
    // {
    //     huan2_leijia_por += right_num;
    //     if (huan2_leijia_por > 13000)
    //     {
    //         huan2_flag = 0;
    //         huan2_leijia_por = 0;
    //     }
    // }
    // else
    //     huan2_leijia_por = 0;

    // if (g_fSpeedControlOutNew >= 0) // 判断正反(正) 0
    // {
    //     right_zheng_ma = right_zheng_ma + right_num;
    // }
    // else //   (反)
    // {
    //     right_fan_ma = right_fan_ma + right_num;
    // }

    // gpt12_clear(GPT12_T6);
    // gpt12_clear(GPT12_T4);
}

// float icm_angle(int16 icm_acc_x, int16 icm_acc_z, int16 icm_gyro_y) //获取陀螺仪角度  用不到
//{
//
//
//
//       Gyro_Y=icm_gyro_y;
//       Accel_X=icm_acc_x;
//       Accel_Z=icm_acc_z;
//
//                         if(Gyro_Y>32768)  Gyro_Y-=65536;                       //数据类型转换  也可通过short强制类型转换
//                         if(Accel_X>32768) Accel_X-=65536;                      //数据类型转换
//                         if(Accel_Z>32768) Accel_Z-=65536;                      //数据类型转换
//
//                         Accel_Y=atan2(Accel_X,Accel_Z)*180/3.14159265;                 //加速度计计算倾角
//                         Gyro_Y=Gyro_Y/16.4;    //Angle = gyro * dt   dt是采样时间越短积分值越准确
//
//                         angle2 = K1 * Accel_Y+ (1-K1) * (angle2 + Gyro_Y * K2);
//
//
// }

void speed_get()
{

    int y = 0;
    left_whitenum = 0;
    right_whitenum = 0;

    if (Set == 0)
        Set_Speed1 = 560;
    else if (Set == 1)
        Set_Speed1 = 570;
    else if (Set == 2)
        Set_Speed1 = 580;
    else if (Set == 3)
        Set_Speed1 = 590;
    else if (Set == 4)
        Set_Speed1 = 600;
    else if (Set == 5)
        Set_Speed1 = 610;
    else if (Set == 6)
        Set_Speed1 = 620;
    else if (Set == 7)
        Set_Speed1 = 630;
    else if (Set == 8)
        Set_Speed1 = 640;
    else if (Set == 9)
        Set_Speed1 = 650;

    for (y = 69; y >= 0; y--)
    {
        if (image_use[y][left[y]] == 0)
        {
            break;
        }
        else
        {
            left_whitenum++;
        }
    }
    for (int y = 69; y >= 0; y--)
    {
        if (image_use[y][right[y]] == 0)
        {
            break;
        }
        else
        {
            right_whitenum++;
        }
    }

    if (left_whitenum < right_whitenum)
        speed_yingshe = left_whitenum;
    else
        speed_yingshe = right_whitenum;

    zhidao_juli = 0.001 * (float)speed_yingshe * speed_yingshe * speed_yingshe - 0.0603 * (float)speed_yingshe * speed_yingshe + 1.6174 * (float)speed_yingshe - 2.9669;
    if (zhidao_juli <= 1)
        zhidao_juli = 1;
    if (((zhidao_juli > 135 && zhidao_num < 29) && (zuodiuxianshu <= 3) && (youdiuxianshu <= 3) && (Right_Add_num <= 3) && (Left_Add_num <= 3)) && !(left_huan_num == 2 || left_huan_num == 3) && !(right_huan_num == 2 || right_huan_num == 3) && !youhuihuan_flag && !lefthuihuan_flag && park_flag != 2 && poer_flag == 0 && por_cnt == 0)
    {
        yuzhi_speed = Set_Speed2 - 100;
        jiasu_part = 0;
        zhidao_flag = 1;
        // jiasha_part=0;
    }
    else if (wailun_speed > com_top)
    {
        yuzhi_speed = com_top;
        julu_speed = 0;
        zhidao_flag = 0;
        jisha_flag = 1;
    }

    else if (wailun_speed > por_top && zhidao_num > 29) // 坡
    {
        yuzhi_speed = por_top;
        zhidao_flag = 0;
        jisha_flag = 2;
    }
    else if (wailun_speed > huan_top && (left_huan_num == 1 || left_huan_num == 2 ||
                                         right_huan_num == 1 || right_huan_num == 2 || poer_flag || por_cnt || zhidao_num > 29 || huan2_flag)) // 环岛回环
    {
        yuzhi_speed = huan_top;
        zhidao_flag = 0;
        jisha_flag = 3;
    }

    else if ((xiaozhi && huihuan_num > 23 && zhidao_num < 29) //||huihuan_num>23zhidao_juli>90
             && !(left_huan_num == 1 || left_huan_num == 2 || left_huan_num == 3 || left_huan_num == 4 || left_huan_num == 5 || left_huan_num == 6 || left_huan_num == 7 ||
                  right_huan_num == 1 || right_huan_num == 2 || right_huan_num == 3 || right_huan_num == 4 || right_huan_num == 5 || right_huan_num == 6 || right_huan_num == 7) &&
             !youhuihuan_flag && !lefthuihuan_flag && poer_flag == 0 && por_cnt == 0)
    {
        yuzhi_speed = Set_Speed2;
        jiasu_part = 20;
        jiasha_part = 0;
        jisha_flag = 0;
        zhidao_flag = 2;
    }
    else
    {
        yuzhi_speed = Set_Speed2;
        jisha_flag = 0;
        zhidao_flag = 0;
        jiasu_part = 0.0015822 * zhidao_juli * zhidao_juli;
    }
    if (yuzhi_speed < 100)
        yuzhi_speed = 100;
    if (chujie == 1 || tingche_flag2)
    {
        Set_Speed2 = 0;
    }
    else if (park_flag == 2 && !tingche_flag && tingche_way == 1) //
    {
        Set_Speed2 = 100;
    }
    else if (park_flag == 2 && tingche_flag && tingche_way == 1) //
    {
        Set_Speed2 = 50;
    }
    else if (park_flag == 2 && !tingche_flag && tingche_way == 2)
    {
        Set_Speed2 = 540;
    }
    else if (park_flag == 2 && tingche_flag && tingche_way == 2)
    {
        Set_Speed2 = 250;
    }
    else
    {
        if (junsu == 0)
        {
            if (jisha_flag == 0)
            {
                if (left_huan_num == 1 || right_huan_num == 1)
                {
                    Set_Speed2 = yanhuan_sd;
                }
                else if (left_huan_num == 2 || right_huan_num == 2)
                {
                    Set_Speed2 = yanhuan_sd - yanhuan_sd * 0.07;
                }
                else if (left_huan_num == 3 || left_huan_num == 4 || right_huan_num == 4 ||
                         left_huan_num == 5 || right_huan_num == 5 || right_huan_num == 3)
                    Set_Speed2 = yanhuan_sd - yanhuan_sd * 0.17;
                else if (
                    left_huan_num == 6 || right_huan_num == 6)
                    Set_Speed2 = yanhuan_sd - yanhuan_sd * 0.05;

                else if (three_cross1 || three_cross)
                {
                    Set_Speed2 = Set_Speed1 - Set_Speed1 * 0.08;
                }

                else if (poer_flag || por_cnt) //(||star_lineflag&&park_flag==1)
                    Set_Speed2 = por_sd;
                else if (zhidao_flag == 1)
                    Set_Speed2 = Set_Speed1 + zhidao_jia;
                else if (zhidao_flag == 2)
                    Set_Speed2 = Set_Speed1 - jiasha_part;
                else if (lefthuihuan_flag == 1 || lefthuihuan_flag == 2)
                    Set_Speed2 = huihuan_sd - huihuan_sd * 0.10;
                else if (youhuihuan_flag == 1 || youhuihuan_flag == 2)
                    Set_Speed2 = huihuan_sd - huihuan_sd * 0.10;
                else if (youhuihuan_flag == 3)
                    Set_Speed2 = huihuan_sd - huihuan_sd * 0.17; // 575
                else if (lefthuihuan_flag == 3)
                    Set_Speed2 = huihuan_sd - huihuan_sd * 0.17;
                else
                    Set_Speed2 = Set_Speed1 - jiasu_part; //
            }
            else
                Set_Speed2 = yuzhi_speed;
        }
        else
        {
            if (left_huan_num == 1 || right_huan_num == 1)
            {
                Set_Speed2 = yanhuan_sd;
            }
            else if (left_huan_num == 2 || right_huan_num == 2)
            {
                Set_Speed2 = yanhuan_sd - yanhuan_sd * 0.07;
            }
            else if (left_huan_num == 3)
                Set_Speed2 = yanhuan_sd - yanhuan_sd * 0.14;
            else if (right_huan_num == 3)
                Set_Speed2 = yanhuan_sd - yanhuan_sd * 0.14;
            else if (left_huan_num == 4 || right_huan_num == 4 ||
                     left_huan_num == 5 || right_huan_num == 5 ||
                     left_huan_num == 6 || right_huan_num == 6)
                Set_Speed2 = yanhuan_sd - yanhuan_sd * 0.05;
            else if (three_cross1 || three_cross)
            {
                Set_Speed2 = Set_Speed1 - Set_Speed1 * 0.08;
            }

            else if (poer_flag || por_cnt) //(||star_lineflag&&park_flag==1)
                Set_Speed2 = por_sd;
            else if (zhidao_flag == 1)
                Set_Speed2 = Set_Speed1 + zhidao_jia;
            else if (lefthuihuan_flag == 1 || lefthuihuan_flag == 2)
                Set_Speed2 = huihuan_sd - huihuan_sd * 0.10;
            else if (youhuihuan_flag == 1 || youhuihuan_flag == 2)
                Set_Speed2 = huihuan_sd - huihuan_sd * 0.10;
            else if (youhuihuan_flag == 3)
                Set_Speed2 = huihuan_sd - huihuan_sd * 0.15; // 575
            else if (lefthuihuan_flag == 3)
                Set_Speed2 = huihuan_sd - huihuan_sd * 0.15;
            else
                Set_Speed2 = Set_Speed1;
        }
        //   8           5                      3*比例
    }
}

void ruku_handle()
{
    unsigned int ruku_lie = 8;

    for (int i = 67; i >= 5; i--)
    {
        if (image_use[i][ruku_lie] == 0 && image_use[i - 1][ruku_lie] == 0 && image_use[i - 2][ruku_lie] == 0 && image_use[i + 1][ruku_lie] == 255 && image_use[i + 2][ruku_lie] == 255)
        {
            ruku_zuobiao_hang_jubu = i;
            break;
        }
    }

    if (ruku_zuobiao_hang_jubu)
    {
        for (int i = ruku_lie; i < 170; i += 2)
        {
            if (image_use[ruku_zuobiao_hang_jubu + 1][i] == 255 && image_use[ruku_zuobiao_hang_jubu + 2][i] == 255 && image_use[ruku_zuobiao_hang_jubu - 1][i] == 255 && image_use[ruku_zuobiao_hang_jubu - 2][i] == 255 && image_use[ruku_zuobiao_hang_jubu][i + 1] == 255 && image_use[ruku_zuobiao_hang_jubu][i + 2] == 255)
            {
                ruku_zuobiao_lie = i - 3;
                break;
            }
        }
    }

    for (int i = ruku_zuobiao_hang_jubu - 3; i < 70; i++)
    {
        if (image_use[i][ruku_zuobiao_lie] == 0 && image_use[i + 1][ruku_zuobiao_lie] == 255 && image_use[i + 2][ruku_zuobiao_lie] == 255)
        {
            ruku_zuobiao_hang = i;
            break;
        }
    }

    find_leftdown_point(67, 15, 1); // 1是十字
    find_rightdown_point(67, 15, 1);
    if (ruku_zuobiao_lie && ruku_zuobiao_hang && ruku_zuobiao_hang > check_line)
        // if(left_turn_down[0]>50&&left_turn_down[0]!=69)
        tingche_flag = 1;
}

void ruku_handle2()
{
    unsigned int ruku_lie = 185;

    for (int i = 67; i >= 1; i--)
    {
        if (image_use[i][ruku_lie] == 0 && image_use[i - 1][ruku_lie] == 0 && image_use[i - 2][ruku_lie] == 0 && image_use[i + 1][ruku_lie] == 255 && image_use[i + 2][ruku_lie] == 255)
        {
            ruku_youbiao_hang_jubu = i - 5;
            break;
        }
    }

    if (ruku_youbiao_hang_jubu)
    {
        for (int i = ruku_lie; i > 10; i--)
        {
            if (image_use[ruku_youbiao_hang_jubu + 1][i] == 255 && image_use[ruku_youbiao_hang_jubu + 2][i] == 255 && image_use[ruku_youbiao_hang_jubu - 1][i] == 255 && image_use[ruku_youbiao_hang_jubu - 2][i] == 255 && image_use[ruku_youbiao_hang_jubu][i - 1] == 255 && image_use[ruku_youbiao_hang_jubu][i - 2] == 255)
            {
                ruku_youbiao_lie = i + 5;
                break;
            }
        }
    }

    for (int i = ruku_youbiao_hang_jubu; i < 70; i++)
    {
        if (image_use[i - 1][ruku_youbiao_lie] == 0 && image_use[i][ruku_youbiao_lie] == 0 && image_use[i + 1][ruku_youbiao_lie] == 255 && image_use[i + 2][ruku_youbiao_lie] == 255)
        {
            ruku_youbiao_hang = i;
            break;
        }
    }

    //  find_leftdown_point(67,15,1);   //1是十字
    //   find_rightdown_point(67,15,1);

    if (ruku_youbiao_lie && ruku_youbiao_hang && ruku_youbiao_hang > check_line)
        // if(left_turn_down[0]>50&&left_turn_down[0]!=69)
        tingche_flag = 1;
}

void chujiebaohu()
{
    chujie_num = 0;
    for (int x = 40; x <= 144; x++)
    {
        if (image_use[68][x] == 0) // ???????????68?а?????????С??????  ?ж??????
        {
            chujie_num++;
        }
    }
}
