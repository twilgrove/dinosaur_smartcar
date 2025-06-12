#include "headfile.h"

#define CAMERA_H 70
#define CAMERA_W 188
float BlackThres = 160.0; // 黑白阈值
int kk, bai_flag = 0, hei_flag = 0, baihei_flag = 0, heibai_flag = 0, width_heibai = 0;
int s1, s2, s3, s4;
int twolines_trend = 2;
int uart_buf[10];
extern long int time_flag; /////////////
long int time_flag1;
int Point_last1 = 95, Point_last2 = 95, Point_last3 = 95, Points = 0;
int qvlv_quanju_right = 0, qvlv_quanju_left = 0, qulv_jinduan_right = 0, qulv_jinduan_left = 0, qulv_yuandaun_right = 0, qulv_yuandaun_left = 0;
int qvlv_quanju = 0, qulv_jinduan = 0, qulv_yuandaun = 0;
int three_cross_cnt = 0;
int center_th[70];
int check_line = 17;
float k_center = 0;
int times2 = 0;
int cnt3 = 0;
int th_y = 0;
int black_blocks = 0;
int cursor = 0; // 指向栈顶的游标thy
int n = 0;
float trend_of_left = 0;
float trend_of_right = 0;
float k_left = 0;
float k_right = 0;
float parameterB = 0, parameterA = 0;
unsigned int Index_Y = 0;
unsigned int Index_Y_last = 0, Index_X_last = 0;
unsigned int Index_X = 0;
int guaidian_leftnum = 0, three_cross = 0, three_cross1 = 0;
float curvity_right = 0;
float curvity_left = 0;
int r_start = 0;
int l_start = 0;
int times = 0;
unsigned int youxie_shizi = 0;
unsigned int zuoxie_shizi = 0;
int findleftdownguai = 0;
int findrightdownguai = 0;
int findrightupguai = 0;
int findleftupguai = 0;
unsigned int xk = 0, xj = 0;
int guaidian;

unsigned int quanzhi_num = 0;
/***偏差权重***/
const unsigned int Weight[70] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端60——70行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端   0——10行权重
        1, 1, 1, 1, 2, 3, 4, 6, 8, 11,          // 图像最远端10——20行权重
        15, 17, 18, 21, 20, 19, 18, 17, 16, 15, // 图像最远端20——30行权重
        13, 12, 11, 10, 9, 8, 7, 6, 5, 4,       // 图像最远端30——40行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端40——50行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端50——60行权重
}; // 69

const unsigned int Weight_jtai[26] =
    {
        2, 3, 4, 6, 8, 9,                       // 图像最远端10——20行权重
        10, 11, 11, 12, 13, 14, 15, 20, 22, 25, // 图像最远端20——30行权重
        30, 33, 37, 40, 30, 20, 10, 8, 5, 4,    // 图像最远端30——40行权重
                                                // 图像最远端50——60行权重
}; // 69

const unsigned int Weight_huandao[70] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端60——70行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端0——10行权重
        1, 1, 1, 1, 2, 3, 4, 6, 8, 11,          // 图像最远端10——20行权重
        15, 17, 18, 21, 20, 19, 18, 17, 16, 15, // 图像最远端20——30行权重
        13, 12, 11, 10, 9, 8, 7, 6, 5, 4,       // 图像最远端30——40行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端40——50行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端50——60行权重

}; // 69
const unsigned int Weight_huihuan[70] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 3, 4, 6, 8, 11,
    15, 17, 18, 21, 20, 19, 18, 17, 16, 15,
    13, 12, 11, 10, 9, 8, 7, 6, 5, 4,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int huihuan_num = 0, zhidao_num = 0, huandao_7 = 0;

unsigned int weight_jubu[70] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 图像最远端60——70行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 图像最远端0——10行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 图像最远端10——20行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 图像最远端20——30行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 图像最远端30——40行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 图像最远端40——50行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 图像最远端50——60行权重;
};
const unsigned int Weight_park[70] =
    {
        2, 3, 4, 6, 8, 11, 15, 17, 18, 21,
        20, 19, 18, 17, 16, 15, 13, 12, 11, 10,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0}; // 69
int j_flag = 0;

int huihuan_left = 0;
int huihuan_right = 0;
unsigned int bai_flag2 = 0, hei_flag2 = 0, baihei_flag2 = 0, heibai_flag2 = 0;
unsigned int stop_you = 0, stop_zuo = 0;
long long int Sum = 0, Weight_Count = 0;
int stop_num1 = 0, stop_num2 = 0;
int park_flag = 0; // 停车标志
int y = 0;
unsigned int Foresight_Left = 0;    ////左侧最小赛道宽度
unsigned int Foresight_Right = 186; ////右侧最小赛道宽度
int Point_Mid = 0, Foresight = 0;
double Get_Point;
unsigned char image_use[70][188];

int whitenum = 0;
//

////直接使用的
unsigned int Half_width[70] =
    {
        00,
        00,
        00,
        00,
        00,
        17,
        18,
        19,
        20,
        22,
        22,
        23,
        25,
        26,
        27,
        28,
        29,
        30,
        31,
        33,
        34,
        35,
        36,
        37,
        38,
        39,
        40,
        41,
        42,
        43,
        44,
        45,
        46,
        47,
        48,
        49,
        50,
        51,
        52,
        53,
        54,
        56,
        57,
        58,
        59,
        60,
        61,
        62,
        63,
        64,
        65,
        66,
        67,
        69,
        70,
        71,
        72,
        74,
        75,
        76,
        77,
        78,
        79,
        80,
        81,
        82,
        83,
        84,
        82,
        82,
};

////用于动态调整的
unsigned int const Half_width2[70] =
    {
        00,
        00,
        00,
        00,
        00,
        17,
        18,
        19,
        20,
        22,
        22,
        23,
        25,
        26,
        27,
        28,
        29,
        30,
        31,
        33,
        34,
        35,
        36,
        37,
        38,
        39,
        40,
        41,
        42,
        43,
        44,
        45,
        46,
        47,
        48,
        49,
        50,
        51,
        52,
        53,
        54,
        56,
        57,
        58,
        59,
        60,
        61,
        62,
        63,
        64,
        65,
        66,
        67,
        69,
        70,
        71,
        72,
        74,
        75,
        76,
        77,
        78,
        79,
        80,
        81,
        82,
        83,
        84,
        82,
        82,
};

////直接使用的
unsigned int Half_width_handao[70] = // time_flag
    {
        00,
        00,
        00,
        00,
        00,
        17,
        18,
        19,
        20,
        22,
        22,
        23,
        25,
        26,
        27,
        28,
        29,
        30,
        31,
        33,
        34,
        35,
        36,
        37,
        38,
        39,
        40,
        41,
        42,
        43,
        44,
        45,
        46,
        47,
        48,
        49,
        50,
        51,
        52,
        53,
        54,
        56,
        57,
        58,
        59,
        60,
        61,
        62,
        63,
        64,
        65,
        66,
        67,
        69,
        70,
        71,
        72,
        74,
        75,
        76,
        77,
        78,
        79,
        80,
        81,
        82,
        83,
        84,
        82,
        82,
};

////用于动态调整的
unsigned int const Half_width_handao2[70] = // time_flag
    {
        00,
        00,
        00,
        00,
        00,
        17,
        18,
        19,
        20,
        22,
        22,
        23,
        25,
        26,
        27,
        28,
        29,
        30,
        31,
        33,
        34,
        35,
        36,
        37,
        38,
        39,
        40,
        41,
        42,
        43,
        44,
        45,
        46,
        47,
        48,
        49,
        50,
        51,
        52,
        53,
        54,
        56,
        57,
        58,
        59,
        60,
        61,
        62,
        63,
        64,
        65,
        66,
        67,
        69,
        70,
        71,
        72,
        74,
        75,
        76,
        77,
        78,
        79,
        80,
        81,
        82,
        83,
        84,
        82,
        82,
};

////直接处理过程中使用的赛道半宽
unsigned int Half_width_yuanshi[70] = {
    00,
    00,
    00,
    00,
    00,
    17,
    18,
    19,
    20,
    22,
    22,
    23,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    56,
    57,
    58,
    59,
    60,
    61,
    62,
    63,
    64,
    65,
    66,
    67,
    69,
    70,
    71,
    72,
    74,
    75,
    76,
    77,
    78,
    79,
    80,
    81,
    82,
    83,
    84,
    82,
    82,
};

int c = 0;
int huan2_flag = 0;
int star_lineflag = 0, star_lineflag2 = 0;

int left_line[70], right_line[70]; // 最长摆列法左边界右边界     [0]  is  x,[1]  is  y
int Left_Add2[70], Right_Add2[70];
int Left_Add[70], Right_Add[70], Left_Add_num = 0, Right_Add_num = 0, Left_Add_num2 = 0, Right_Add_num2 = 0;
int sousuojieshuhang; // 搜索结束行
float Add_Slope = 0, Left_Last_Slope = 0, Right_Last_Slope = 0;

int center[70], Width[70]; // 中线
int youdiuxianshu = 0;     // 右丢线数
int zuodiuxianshu = 0;     // 左丢线数
int zongdiuxianshu = 0;    // 总丢线数

int white_num_col[188], white_num_col_max = 0, white_num_col_line = 0, white_num_col_min_line = 0, white_num_col_min = 69;
float Up_R_qulv;   // 当前赛道前半部分曲率半径
float Down_R_qulv; // 当前赛道后半部分曲率半径
int Left_Add_Start = 0, Left_Add_End = 0;
int Right_Add_Start = 0, Right_Add_End = 0;
int threshold1 = 0, threshold1_old = 77;
int threshold2 = 0, threshold2_old = 77;

int Thresholds[3] = {0};
int xielv_1eft2 = 80;
int xielv_right2 = 80;
int xielv = 22;

int Width_Min = 69;
int Right_y = 0, Left_y = 0;
int Left_Max, Right_Min, Mid_Count;
int Left_Line_New[70], Right_Line_New[70];   // 边界补线的坐标
int Left_Line_New2[70], Right_Line_New2[70]; // 边界补线的坐标
int temp_r = 185;

int curvity_point2 = 0;
int curvity_point1 = 0;
int flag_shizi = 0;
int right_turn_down[2] = {69, 187};
int left_turn_down[2] = {69, 0};
int right_turn_up[2] = {0, 0};
int left_turn_up[2] = {0, 0};

void find_rightdown_point(int start_point, int end_point, int RoadName)
{
    unsigned int j;
    if (RoadName == 1)
    {
        for (j = start_point; j >= end_point; j--)
        {
            // 找右边突变(下拐点)
            if (j >= sousuojieshuhang + 4 && abs(right_line[j + 1] - right_line[j + 2]) <= 3 && abs(right_line[j] - right_line[j + 1]) <= 3 && (right_line[j - 1] - right_line[j]) >= 1 && (right_line[j - 2] - right_line[j]) >= 2 && (right_line[j - 3] - right_line[j]) >= 3 && Right_Add[j + 2] == 0 && Right_Add[j + 1] == 0 && Right_Add[j] == 0)
            {
                right_turn_down[0] = j + 1; // 数组里面没有第0行
                right_turn_down[1] = right_line[j];
                break;
            }
        }
    }
    else if (RoadName == 2)
    {
        for (j = start_point; j >= end_point; j--)
        {
            // 找右边突变(下拐点)
            if (r_start > 65 && j >= sousuojieshuhang + 4 && abs(right_line[j + 1] - right_line[j + 2]) <= 2 && abs(right_line[j] - right_line[j + 1]) <= 2 && (right_line[j - 1] - right_line[j]) >= 1 && (right_line[j - 2] - right_line[j]) >= 2 && (right_line[j - 3] - right_line[j]) >= 3 && Right_Add[j + 2] == 0 && Right_Add[j + 1] == 0 && Right_Add[j] == 0)
            {
                right_turn_down[0] = j + 1; // 数组里面没有第0行
                right_turn_down[1] = right_line[j];
                break;
            }
        }
        // 如果右下拐点坐标不为空 且 环岛状态不是4 就认定找到环岛的右下拐点
        // if (f[0] != 0 && huandao_memory != 4) flag_find_huan_rightdown_point = 1;
    }
    else
    {
    }
}

// 【Function6】找左下拐点函数   abs
// 输入： start点 end点  所要判断的下拐点的类型（环岛还是十字？）
void find_leftdown_point(int start_point, int end_point, int RoadName)
{
    unsigned int j;
    if (RoadName == 1)
    {
        for (j = start_point; j >= end_point; j--)
        {
            // 找左边突变(下拐点)；连续且是存在的点（）
            //            if (j>=sousuojieshuhang+4&&abs(left_line[j + 1] - left_line[j + 2]) >= 2 && abs(left_line[j] - left_line[j + 1]) >=2 && (left_line[j ] - left_line[j-1])>=1&& (left_line[j ] - left_line[j-2])>=2&& (left_line[j ] - left_line[j-3])>=3
            //                && Left_Add[j + 2] == 0 && Left_Add[j + 1] == 0 && Left_Add[j] == 0)
            if (j >= sousuojieshuhang + 4 && abs(left_line[j + 1] - left_line[j + 2]) <= 3 && abs(left_line[j] - left_line[j + 1]) <= 3 && (left_line[j] - left_line[j - 1]) >= 1 && (left_line[j] - left_line[j - 2]) >= 2 && (left_line[j] - left_line[j - 3]) >= 3 && Left_Add[j + 2] == 0 && Left_Add[j + 1] == 0 && Left_Add[j] == 0)
            {

                left_turn_down[0] = j; // 数组里面没有第0行
                left_turn_down[1] = left_line[j];
                break;
            }
        }
    }
    else if (RoadName == 2)
    {
        // setText用户自定义("l_start"+ l_start);
        for (j = start_point; j >= end_point; j--)
        {
            // 找左边突变(下拐点)；连续且是存在的点（）
            if (l_start > 65 && j >= sousuojieshuhang + 4 && abs(left_line[j + 1] - left_line[j + 2]) <= 2 && abs(left_line[j] - left_line[j + 1]) <= 2 && (left_line[j] - left_line[j - 1]) >= 1 && (left_line[j] - left_line[j - 2]) >= 2 && (left_line[j] - left_line[j - 3]) >= 3 && Left_Add[j + 2] == 0 && Left_Add[j + 1] == 0 && Left_Add[j] == 0)
            {

                left_turn_down[0] = j; // 数组里面没有第0行
                left_turn_down[1] = left_line[j];
                break;
            }
        }
        //    if (left_turn_down[0] != 0 && huandao_memory != 4) flag_find_huan_leftdown_point = 1;
    }
    else
    {
    }
}

unsigned int my_sqrt(int x)
{
    unsigned int ans = 0, p = 0x80;
    while (p != 0)
    {
        ans += p;
        if (ans * ans > x)
        {
            ans -= p;
        }
        p = (unsigned int)(p / 2);
    }
    return (ans);
}
float process_curvity(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3)
{
    float K;
    int S_of_ABC = ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2;
    // 面积的符号表示方向
    char q1 = (char)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    char AB = my_sqrt(q1);
    q1 = (char)((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));
    char BC = my_sqrt(q1);
    q1 = (char)((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
    char AC = my_sqrt(q1);
    if (AB * BC * AC == 0)
    {
        K = 0;
    }
    else
        K = (float)4 * S_of_ABC / (AB * BC * AC);
    return K;
}

char oppositeSigns(int x, int y)
{
    return ((x ^ y) < 0);
}
void Cal_losttimes(int times)
{
    unsigned int i;
    unsigned int flag_of_rightbreak = 0;
    unsigned int flag_of_leftbreak = 0;
    if (times < 20)
        times = 20;
    for (i = 67; i > times; i--)
    {
        // 左线操作
        if (Left_Add[i] == 1) // 未扫到线
        {
            zuodiuxianshu++;
            if (flag_of_leftbreak == 0) // 如果在这一行之前没有遭遇断线，则计数
            {
                l_start = i; ////l_start首次丢线行
            }
        }
        else // 扫到线
        {
            // lostleft_times不继续增加
            flag_of_leftbreak = 1; // break标志成立
        }
        // 右线操作
        if (Right_Add[i] == 1) // 未扫到线
        {
            youdiuxianshu++;
            if (flag_of_rightbreak == 0) // 如果在这一行之前没有遭遇断线，则计数
            {
                r_start = i;
            }
        }
        else // 扫到线
        {
            // lostright_times不继续增加
            flag_of_rightbreak = 1; // break标志成立
        }
    }
}
int car_flag = 0;
void Center_line_deal() // 中线处理
{
    k_center = 0;
    qvlv_quanju_right = qvlv_quanju_left = qulv_jinduan_right = qulv_jinduan_left = qulv_yuandaun_right = qulv_yuandaun_left = 0;
    k_left = 0;
    k_right = 0;
    Width_Min = 90;
    whitenum = 0;
    right_turn_down[0] = 69;
    left_turn_down[0] = 69;
    right_turn_down[1] = 187;
    left_turn_down[1] = 1;
    xielv_1eft2 = 80;
    xielv_right2 = 80;
    findleftdownguai = 0;
    findrightdownguai = 0;
    findrightupguai = 0;
    findleftupguai = 0;
    times = 0;
    Foresight_Left = 0, Foresight_Right = 186;
    youdiuxianshu = 0;
    zuodiuxianshu = 0;
    Left_Add_Start = 0;
    Right_Add_Start = 0;
    Left_Add_End = 0;
    Right_Add_End = 0;
    white_num_col_max = 0;
    Left_Add_num = 0, Right_Add_num = 0;
    Left_Add_num2 = 0, Right_Add_num2 = 0;
    white_num_col_min = 69;
    l_start = 68;
    r_start = 68;

    for (int y = 0; y < 70; ++y)
    {
        for (int x = 0; x < 188; ++x)
        {
            image_use[y][x] = Img_Store_pp->Img_OTSU.at<uchar>(y, x); // 值为 0 或 255
            // std::cout<<image_use[y][x]<<std::endl;
            // std::cout<<Img_Store_pp->Img_OTSU.cols<<","<<Img_Store_pp->Img_OTSU.rows<<std::endl;
        }
    }

    for (int ql = 0; ql <= 69; ql++) // 清零函数
    {
        left_line[ql] = 2;     ////存储左边界线的列坐标
        Left_Add[ql] = 0;      ////标记左边界是否需要基础补线
        Left_Add2[ql] = 0;     ////标记左边界是否需要高级补线（动态斜率补线标志）
        Left_Line_New[ql] = 2; ////存储修复后的左边界线（最终使用的左边界）
    }
    for (int ql = 0; ql <= 187; ql++) // 清零函数guaidian
    {
        white_num_col[ql] = 0; ////白列白点数
    }
    for (int ql = 0; ql <= 69; ql++)
    {
        right_line[ql] = 184;     ////存储右边界线的列坐标
        Right_Line_New[ql] = 184; ////存储修复后的右边界线（最终使用的右边界）
        center[ql] = 89;          // 每行中线点
        Right_Add[ql] = 0;        ////标记右边界是否需要基础补线
        Right_Add2[ql] = 0;       ////标记右边界是否需要高级补线（动态斜率补线标志）
    }

    int x = 0, y = 0; // 设x为行，y为列
    unsigned int temp = 0;
    ////////////////////////////扫描最底下3行/////////////////////////
    ////寻找最长白列
    if (left_huan_num == 3) // right_huan_num==6||
    {
        for (x = 184; x > 101; x--)
        {
            for (y = 69; y >= 0; y--) ////更新white_num_col[x]
            {
                if (image_use[y][x] == 0)
                {
                    break;
                }
                else
                {
                    white_num_col[x]++;
                }
            }
            if (white_num_col[x] > white_num_col_max)
            {
                white_num_col_max = white_num_col[x]; ////white_num_col_max最大白列白点数
                white_num_col_line = x;               ////white_num_col_line最长白列
            }
        }
    }
    else if (right_huan_num == 3) //||left_huan_num==6
    {
        for (x = 18; x < 101; x++)
        {
            for (y = 69; y >= 0; y--)
            {
                if (image_use[y][x] == 0)
                {
                    break;
                }
                else
                {
                    white_num_col[x]++;
                }
            }
            if (white_num_col[x] > white_num_col_max)
            {
                white_num_col_max = white_num_col[x];
                white_num_col_line = x;
            }
        }
    }
    else
    {
        if (park_flag == 1)
        {
            for (x = 18; x <= 100; x++)
            {
                for (y = 69; y >= 0; y--)
                {
                    if (image_use[y][x] == 0)
                    {
                        break;
                    }
                    else
                    {
                        white_num_col[x]++;
                    }
                }
                if (white_num_col[x] > white_num_col_max)
                {
                    white_num_col_max = white_num_col[x];
                    white_num_col_line = x;
                } // k_right
            }
            for (x = 184; x >= 101; x--)
            {
                for (y = 69; y >= 0; y--)
                {
                    if (image_use[y][x] == 0)
                    {
                        break;
                    }
                    else
                    {
                        white_num_col[x]++;
                    }
                }
                if (white_num_col[x] > white_num_col_max)
                {
                    white_num_col_max = white_num_col[x];
                    white_num_col_line = x;
                }
            }
        }
        if (park_flag == 0)
        {
            for (int x = 184; x >= 101; x--)
            {
                for (int y = 69; y >= 0; y--)
                {
                    if (image_use[y][x] == 0)
                    {
                        break;
                    }
                    else
                    {
                        white_num_col[x]++;
                    }
                }
                if (white_num_col[x] > white_num_col_max)
                {
                    white_num_col_max = white_num_col[x];
                    white_num_col_line = x;
                }
            }
            for (int x = 18; x <= 100; x++)
            {
                for (int y = 69; y >= 0; y--)
                {
                    if (image_use[y][x] == 0)
                    {
                        break;
                    }
                    else
                    {
                        white_num_col[x]++;
                    }
                }
                if (white_num_col[x] > white_num_col_max)
                {
                    white_num_col_max = white_num_col[x];
                    white_num_col_line = x;
                } // k_right
            }
        }
        else if (tingche_flag)
        {
            if (c_r == 2)
            {
                for (x = 18; x <= 115; x++)
                {
                    for (y = 69; y >= 0; y--)
                    {
                        if (image_use[y][x] == 0)
                        {
                            break;
                        }
                        else
                        {
                            white_num_col[x]++;
                        }
                    }
                    if (white_num_col[x] > white_num_col_max)
                    {
                        white_num_col_max = white_num_col[x];
                        white_num_col_line = x;
                    } // k_right
                }
            }
            else if (c_r == 1)
            {
                for (x = 184; x >= 85; x--)
                {
                    for (y = 69; y >= 0; y--)
                    {
                        if (image_use[y][x] == 0)
                        {
                            break;
                        }
                        else
                        {
                            white_num_col[x]++;
                        }
                    }
                    if (white_num_col[x] > white_num_col_max)
                    {
                        white_num_col_max = white_num_col[x];
                        white_num_col_line = x;
                    } // k_right
                }
            }
        }
    }

    ////最长白列法寻找边界
    for (y = 68; y > 1; y--) // x是减59—56  num是加0—4   ////白点为边界
    {
        ////最长摆列法右边界
        for (int x = white_num_col_line; x <= 184; x++) // 中间向右找跳变
        {
            if (image_use[y][x - 1] == 255 && image_use[y][x] == 255 && image_use[y][x + 1] == 0 && image_use[y][x + 2] == 0) // 两个连续黑点触发
            {
                right_line[y] = x;
                Right_Line_New[y] = x;
                Right_Line_New2[y] = x;
                break;
            }
            else if (x == 184) // 到最右边了都没扫到黑点a
            {
                right_line[y] = x;
                Right_Line_New[y] = x;
                Right_Line_New2[y] = x;
                Right_Add[y] = 1;
                break;
            }
        }
        ////最长摆列法左边界
        for (int x = white_num_col_line; x >= 1; x--) // 中间向左找跳变
        {
            if (image_use[y][x - 2] == 0 && image_use[y][x - 1] == 0 && image_use[y][x] == 255 && image_use[y][x + 1] == 255) // 两个连续黑点触发
            {
                left_line[y] = x;
                Left_Line_New[y] = x;
                Left_Line_New2[y] = x;
                break;
            }
            else if (x == 3) // 到最左边了都没扫到黑点
            {

                left_line[y] = x;
                Left_Line_New[y] = x;
                Left_Line_New2[y] = x;
                Left_Add[y] = 1;

                break;
            }
        }
        //   Half_width[y]=abs(right_line[y]-left_line[y])/2;
        Width[y] = abs(right_line[y] - left_line[y]); ////Width赛道宽度
        // std::cout<<"Y:"<<y<<"   "<<right_line[y] - left_line[y]<<std::endl;
        ////中线处理
        if (left_line[y] <= 3 && right_line[y] < 184)
        {
            if (right_line[y] - Half_width_yuanshi[y] <= 3)
                center_th[y] = 3;
            else
                center_th[y] = right_line[y] - Half_width_yuanshi[y];
        }
        else if (left_line[y] > 3 && right_line[y] >= 184)
        {
            if (left_line[y] + Half_width_yuanshi[y] >= 184)
                center_th[y] = 184;
            else
                center_th[y] = left_line[y] + Half_width_yuanshi[y];
        }
        else
            center_th[y] = (left_line[y] + right_line[y]) / 2;

        ////赛道变宽判断
        if (Width[y] >= Width[y + 1] || (Width[y] >= Width_Min))
        {

            if (Left_Add2[y + 1])
            {
                if (left_line[y] < Left_Line_New2[y + 1] - 1) // 与前一行的左边界实线比较
                {
                    Left_Add2[y] = 1;
                }
            }
            else // 前一行没有补线
            {
                if (left_line[y] < left_line[y + 1] - 1) // 与前一行的左边界实线比较
                {
                    Left_Add2[y] = 1; ////更新Left_Add2[]
                }
            }
            if (Right_Add2[y + 1])
            {
                if (right_line[y] > Right_Line_New2[y + 1] + 1)
                {
                    Right_Add2[y] = 1; ////更新Right_Add2[]
                }
            }
            else // 前一行右边界没有补线
            {
                if (right_line[y] > right_line[y + 1] + 1)
                {
                    Right_Add2[y] = 1;
                }
            }
        }
        if (Left_Add2[y]) // 左边需要补线
        {

            if (y < 62) // 因为有下面+6限制
            {
                if (!Left_Add_Start) // 如果还没有记录开始补线位置
                {
                    Left_Add_Start = y; // 记录左边界补线开始位置
                }
                Add_Slope = 1.0 * (left_line[Left_Add_Start + 6] - left_line[Left_Add_Start + 1]) / 5; // 计算能识别的前几行图像斜率

                // std::cout << "left_line[Left_Add_Start + 6]: " << left_line[Left_Add_Start + 6] << "   left_line[Left_Add_Start + 1]: " << left_line[Left_Add_Start + 1] << "   Add_Slope: " << Add_Slope << std::endl;
                if (Add_Slope > 0) // 限幅
                {
                    Add_Slope = 0;
                }
                temp = (int)((y - (Left_Add_Start + 1)) * Add_Slope + left_line[Left_Add_Start + 1]); // 通过斜率推算补线的位置
                Left_Last_Slope = Add_Slope;                                                          // 更新上次左边界斜率

                Left_Line_New2[y] = range_protect(temp, 2, 184); // 不直接修改边界，只保存在补线数组里
                // std::cout << "Left_Add_Start: " << Left_Add_Start << "  left:  " << y << "   " << temp << std::endl;
                // std::cout << "get_first: " << (y - (Left_Add_Start + 1)) * Add_Slope << std::endl;
            }
            /* 第一次补线，只记录，不在图像上显示 */
            //
        }
        ////右补线
        if (Right_Add2[y]) // 右边需要补线
        {
            if (y < 62)
            {
                if (!Right_Add_Start) // 如果还没有记录开始补线位置
                {
                    Right_Add_Start = y; // 记录左边界补线开始位置
                }

                Add_Slope = 1.0 * (right_line[Right_Add_Start + 6] - right_line[Right_Add_Start + 1]) / 5; // 计算能识别的前几行图像斜率

                if (Add_Slope < 0) // 限幅
                {
                    Add_Slope = 0;
                }
                temp_r = (int)((y - (Right_Add_Start + 1)) * Add_Slope + right_line[Right_Add_Start + 1]); // 通过斜率推算补线的位置
                Right_Last_Slope = Add_Slope;                                                              // 更细上次右边界斜率

                Right_Line_New2[y] = range_protect(temp_r, 2, 184); // 不直接修改边界，只保存在补线数组里
            }
        }

        ////计算理论补线后重新计算赛道宽度
        if (Left_Add2[y] && Right_Add2[y]) // 两边都需要补线
        {
            Width[y] = Right_Line_New2[y] - Left_Line_New2[y]; // 重新计算本行赛道宽度
        }
        else // 不需要补线或只有一边需要补线
        {
            if (Left_Add2[y]) // 此处最多只有一边会需要补线
            {
                Width[y] = right_line[y] - Left_Line_New2[y]; // 重新计算本行赛道宽度
            }
            else if (Right_Add2[y])
            {
                Width[y] = Right_Line_New2[y] - left_line[y]; // 重新计算本行赛道宽度
            }
            else
            {
                Width[y] = right_line[y] - left_line[y]; // 可能会有干扰，也重新计算一次赛道宽度
            }
            if (Width[y] < Width_Min)
            {
                Width_Min = Width[y]; // 更新最小赛道宽度
            }
            if (left_line[y] > Foresight_Left) // 更新左边界最大值
            {
                Foresight_Left = left_line[y];
            }
            if (right_line[y] < Foresight_Right) // 更新右边界最小值
            {
                Foresight_Right = right_line[y];
            }
        }
        if (y >= 30 && Right_Add[y] == 1 && Left_Add[y] == 1)
            times++;
        ////两条边线相交
        if (abs(right_line[y] - left_line[y]) <= 1)
            break;
        ////边线交叉，中点为黑
        if (image_use[y][center_th[y]] == 0)
            break;
        ////搜索到终点
        if (y <= 1)
            break;
    }

    sousuojieshuhang = y + 2;

    ////统计左右边界丢线zuodiuxianshu l_start   youdiuxianshu r_start
    Cal_losttimes(sousuojieshuhang);

    if (car_flag == 0)
    {
        check_starting_line();
        if (star_lineflag == 1)
        {
            car_flag = 1;
        }
    }
    else if (car_flag == 1)
    {
        check_starting_line();
        if (star_lineflag == 0)
            car_flag = 2;
    }
    else if (car_flag == 2)
    {
        check_starting_line();
        if (star_lineflag == 0)
        {
            car_flag = 3;
        }
    }
    else if (car_flag == 3)
    {
        check_starting_line();
        if (star_lineflag == 1)
        {
            car_flag = 4;
        }
    }
    else if (car_flag == 4)
    {
        check_starting_line();
        if (star_lineflag == 0)
        {
            car_flag = 5;
        }
    }
    else if (car_flag == 5)
    {

        l_target = 0;
        r_target = 0;
    }

    ////统计需要动态斜率补线的右边界行数 Right_Add_num Left_Add_num
    for (y = 68; y > 20; y--)
    {
        if (Right_Add2[y] == 1)
            Right_Add_num++;
        if (Left_Add2[y] == 1)
            Left_Add_num++;
    }
    ////需要基础补线的左右边界行数 Right_Add_num2 Left_Add_num2
    for (y = 68; y > 48; y--)
    {
        if (Right_Add[y] == 1)
            Right_Add_num2++;
        if (Left_Add[y] == 1)
            Left_Add_num2++;
    }

    huihuan_num = 0; // 纵向较大白列>66的数目
    huandao_7 = 0;   //>68
    zhidao_num = 0;  //>41
    ////计算纵向较大白列huihuan_num的数目
    for (int x = right_line[68]; x >= left_line[68]; x--)
    {
        if (white_num_col[x] > 66)
            huihuan_num++;
    }
    ////计算纵向大白列zhidao_num和小白列huandao_7的数目
    for (int x = 118; x >= 80; x--)
    {
        if (white_num_col[x] > 68)
            zhidao_num++;
        if (white_num_col[x] > 41)
            huandao_7++;
    }
    if (l_start >= 55 || r_start >= 55)
    {
        find_leftdown_point(67, 15, 1); // 1是十字////left_turn_down[0]存储左拐点Y  left_turn_down[1]存储左拐点X
        find_rightdown_point(67, 15, 1);
    }
    regression(1, 20, 68);
    k_left = parameterB; ////左边界全局斜率拟合

    regression(2, 20, 68); ////右边界全局斜率拟合
    k_right = parameterB;

    // 直接处理的中线
    for (int iqq = 68; iqq > 20; iqq--)
    {

        cv::circle(get_color, cv::Point(center_th[iqq], iqq), 1, cv::Scalar(255, 0, 0), -1); // -1 表示实心圆
    }

    if (!left_huan_num && !right_huan_num)
    {
        // 如果找到左下或者右下拐点，此时检测左右线趋势

        // 时时注意拐点坐标第一个是行数+1，处理时要做减一处理
        if (left_turn_down[0] != 69 || right_turn_down[0] != 69)
        {
            if (left_turn_down[0] != 69 && right_turn_down[0] == 69) // 左下拐点存在而右下拐点不存在
            {
                regression(1, left_turn_down[0] - 3, left_turn_down[0] + 2); // 左线
                trend_of_left = parameterB;
                regression(2, left_turn_down[0] - 3, left_turn_down[0] + 2); // 右线
                trend_of_right = parameterB;
            }
            else if (right_turn_down[0] != 69 && left_turn_down[0] == 69) // 右下拐点存在而左下拐点不存在
            {
                regression(1, right_turn_down[0] - 3, right_turn_down[0] + 2); // 左线
                trend_of_left = parameterB;
                regression(2, right_turn_down[0] - 3, right_turn_down[0] + 2); // 右线
                trend_of_right = parameterB;
            }
            else if (left_turn_down[0] != 69 && left_turn_down[0] != 69) // 左右拐点均存在
            {
                regression(1, left_turn_down[0] - 3, left_turn_down[0] + 2); // 左线
                trend_of_left = parameterB;
                regression(2, right_turn_down[0] - 3, right_turn_down[0] + 2); // 右线
                trend_of_right = parameterB;
            }
        }
        if ((trend_of_left > 0 && trend_of_right < 0) || (trend_of_left < 0 && trend_of_right > 0))
            twolines_trend = 1; /////左右边界趋势相反（十字特征）
        else
            twolines_trend = 0; ////趋势冲突

        if ((left_turn_down[0] != 69 && twolines_trend == 1) || (youdiuxianshu >= 15 && left_turn_down[0] != 69))
        {

            findleftdownguai = 1; // 表示找到左下拐点了
        }
        else
            findleftdownguai = 0;
        if ((right_turn_down[0] != 69 && twolines_trend == 1) || (zuodiuxianshu >= 15 && right_turn_down[0] != 69))
        {

            findrightdownguai = 1; // 表示找到右下拐点了
        }
        else
            findrightdownguai = 0;

        regression(0, 58, 68); ////近端中线拟合
        ////通过近端中线拟合结果扩展到全局
        for (int j = 68; j >= 1; j--)
        {
            int jicun = (int)(parameterB * j + parameterA);
            if (jicun >= 185)
                jicun = 185;
            else if (jicun <= 0)
                jicun = 0;
            center_th[j] = (int)jicun;
        }
        ////中线有效性验证  有效中线点数whitenum  th_y有效中线断点处
        for (int y = 68; y > 0; y--)
        {
            if (image_use[y][center_th[y]] == 0)
            {
                th_y = center_th[y];
                break;
            }
            else
            {
                whitenum++;
            }
        }
    }
    /*************找到左下或右下拐点后，拟合并预测中线，然后再顺着预测后的中线找**************/
    if (findrightdownguai == 1 || findleftdownguai == 1)
    {

        if (findrightdownguai == 1 && findleftdownguai == 0) // 左斜入十字，仅有右下拐点，取右下拐点下的中线行
        {
            if (!three_cross && !three_cross1 && !youhuihuan_flag && !lefthuihuan_flag) // 防三叉
            {
                for (unsigned int j = 68; j >= 1; j--)
                {
                    // 左上拐点
                    if (((j < (unsigned int)left_turn_down[0]) && ((left_line[j] - left_line[j + 3]) >= 10) && ((left_line[j] - left_line[j + 2]) >= 10) && ((left_line[j] - left_line[j + 1]) >= 10)) && Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0)
                    {

                        left_turn_up[0] = j - 2; // 数组里面没有第0行
                        left_turn_up[1] = left_line[j] - 2;
                        // 获得的上坐标先确定一下是不是比下坐标小，如果小则说明提前断掉，此时的“上拐点”为假.
                        // 如果比下坐标大则此时的“上拐点”为真.
                        if (left_turn_up[0] >= left_turn_down[0])
                        {
                            ;
                        }
                        else
                            break;
                    }
                }
                /***找右上拐点***********/
                for (unsigned int j = 68; j >= 1; j--)
                {
                    if (((j < (unsigned int)right_turn_down[0]) && right_line[j + 3] - right_line[j] >= 10 && right_line[j + 2] - right_line[j] >= 10 && right_line[j + 1] - right_line[j] >= 10) && (Right_Add[j] == 0 && Right_Add[j - 1] == 0 && Right_Add[j - 2] == 0))
                    {
                        right_turn_up[0] = j - 2;
                        right_turn_up[1] = right_line[j - 2];
                        if (right_turn_up[0] >= right_turn_down[0])
                        {
                            ;
                        }
                        else
                            break;
                    }
                }
                if (right_turn_up[0] > (sousuojieshuhang) && right_turn_up[0] < right_turn_down[0] && right_turn_up[0] != 0)
                {

                    findrightupguai = 1; // 表示找到右上拐点了
                }
                if (left_turn_up[0] > (sousuojieshuhang) && left_turn_up[0] < left_turn_down[0] && left_turn_up[0] != 0)
                {

                    findleftupguai = 1; // 表示找到左上拐点了
                }

                /*********开始补线(找到左下拐点和左上拐点  或 找到右下拐点和右上拐点)*********/
                if ((findrightupguai == 1 && findrightdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 0))
                {

                    if (findleftupguai == 1 && findleftdownguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                    {
                        unsigned int start1 = left_turn_down[0] + 2;
                        if (start1 >= 68)
                            start1 = 68;

                        unsigned int start2 = left_turn_up[0];
                        if (start2 >= 68)
                            start2 = 68;

                        left_buxian(left_line[start1], start1, left_line[start2], start2);
                    }
                    else if (findleftupguai == 1 && findleftdownguai == 0)
                    {
                        unsigned int start1 = left_turn_up[0];
                        if (start1 >= 68)
                            start1 = 68;

                        left_buxian(2, 68, left_line[start1], start1);
                    }
                    // 并列关系
                    if (findrightdownguai == 1 && findrightupguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                    {
                        unsigned int start1 = right_turn_down[0] + 2;
                        if (start1 >= 68)
                            start1 = 68;

                        unsigned int start2 = right_turn_up[0];
                        if (start2 >= 68)
                            start2 = 68;
                        unsigned int end2 = right_turn_up[0];
                        right_buxian(right_line[start1], start1, right_line[end2], end2);
                    }
                    else if (findrightdownguai == 0 && findrightupguai == 1)
                    {
                        unsigned int start1 = right_turn_up[0];
                        if (start1 >= 68)
                            start1 = 68;

                        right_buxian(184, 68, right_line[start1], start1);
                    }
                }
                else if (!three_cross && right_turn_down[0] < 60) ////?
                    sousuojieshuhang = right_turn_down[0] + 1;
            }
        }
        else if (findrightdownguai == 0 && findleftdownguai == 1) // 右斜入十字，仅有左下拐点，取左下拐点下的中线行
        {
            if (!three_cross && !three_cross1 && !youhuihuan_flag && !lefthuihuan_flag)
            {
                for (unsigned int j = 68; j >= 1; j--)
                {
                    // 左上拐点
                    if (((j < (unsigned int)left_turn_down[0]) && ((left_line[j] - left_line[j + 3]) >= 10) && ((left_line[j] - left_line[j + 2]) >= 10) && ((left_line[j] - left_line[j + 1]) >= 10)) && Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0)
                    {
                        left_turn_up[0] = j - 2; // 数组里面没有第0行
                        left_turn_up[1] = left_line[j] - 2;
                        // 获得的上坐标先确定一下是不是比下坐标小，如果小则说明提前断掉，此时的“上拐点”为假.
                        // 如果比下坐标大则此时的“上拐点”为真.
                        if (left_turn_up[0] >= left_turn_down[0])
                        {
                            ;
                        }
                        else
                            break;
                    }
                }
                /***找右上拐点***********/
                for (unsigned int j = 68; j >= 1; j--)
                {
                    if (((j < (unsigned int)right_turn_down[0]) && right_line[j + 3] - right_line[j] >= 10 && right_line[j + 2] - right_line[j] >= 10 && right_line[j + 1] - right_line[j] >= 10) && (Right_Add[j] == 0 && Right_Add[j - 1] == 0 && Right_Add[j - 2] == 0))
                    {
                        right_turn_up[0] = j - 2;
                        right_turn_up[1] = right_line[j - 2];
                        if (right_turn_up[0] >= right_turn_down[0])
                        {
                            ;
                        }
                        else
                            break;
                    }
                }
                if (right_turn_up[0] > (sousuojieshuhang) && right_turn_up[0] < right_turn_down[0] && right_turn_up[0] != 0)
                {

                    findrightupguai = 1; // 表示找到右上拐点了
                }
                if (left_turn_up[0] > (sousuojieshuhang) && left_turn_up[0] < left_turn_down[0] && left_turn_up[0] != 0)
                {

                    findleftupguai = 1; // 表示找到左上拐点了
                }

                /*********开始补线(找到左下拐点和左上拐点  或 找到右下拐点和右上拐点)*********/
                if ((findleftupguai == 1 && findleftdownguai == 1) || (findleftupguai == 1 && findleftdownguai == 0))
                {

                    if (findleftupguai == 1 && findleftdownguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                    {
                        unsigned int start1 = left_turn_down[0] + 2;
                        if (start1 >= 68)
                            start1 = 68;

                        unsigned int start2 = left_turn_up[0];
                        if (start2 >= 68)
                            start2 = 68;

                        left_buxian(left_line[start1], start1, left_line[start2], start2);
                    }
                    else if (findleftupguai == 1 && findleftdownguai == 0)
                    {
                        unsigned int start1 = left_turn_up[0];
                        if (start1 >= 68)
                            start1 = 68;

                        left_buxian(2, 68, left_line[start1], start1);
                    }
                    // 并列关系
                    if (findrightdownguai == 1 && findrightupguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                    {
                        unsigned int start1 = right_turn_down[0] + 2;
                        if (start1 >= 68)
                            start1 = 68;

                        unsigned int start2 = right_turn_up[0];
                        if (start2 >= 68)
                            start2 = 68;
                        unsigned int end2 = right_turn_up[0];
                        right_buxian(right_line[start1], start1, right_line[end2], end2);
                    }
                    else if (findrightdownguai == 0 && findrightupguai == 1)
                    {
                        unsigned int start1 = right_turn_up[0];
                        if (start1 >= 68)
                            start1 = 68;

                        right_buxian(184, 68, right_line[start1], start1);
                    }
                }
                else if (!three_cross && left_turn_down[0] < 60)
                    sousuojieshuhang = left_turn_down[0] + 1;
            }
        }
        else if (findrightdownguai == 1 && findleftdownguai == 1) // 正入十字：用两个下拐点中最小行下的中线行，拟合出k，b，进而拟合出预测中线
        {
            ////std::cout << "\033[32m找到双拐点\033[0m" << std::endl;

            xielv_1eft2 = abs(left_line[left_turn_down[0] + 5] - left_turn_down[1]) + abs(left_line[left_turn_down[0] - 5] - left_turn_down[1]);
            xielv_right2 = abs(right_line[right_turn_down[0] + 5] - right_turn_down[1]) + abs(right_line[right_turn_down[0] - 5] - right_turn_down[1]);
            if (
                whitenum != 0 && whitenum < 54 &&
                Left_Add_num2 == 0 && Right_Add_num2 == 0 &&
                (!lefthuihuan_flag && !youhuihuan_flag) && ((left_turn_down[0] > 18 && left_turn_down[0] != 69) || (right_turn_down[0] > 18 && right_turn_down[0] != 69)) &&
                abs(left_turn_down[0] - right_turn_down[0]) < 30 &&
                ((xielv_1eft2 < 30 && xielv_right2 < 30) || (xielv_1eft2 < 20 && xielv_right2 > 30) || (xielv_1eft2 > 20 && xielv_right2 < 30)) && xielv_1eft2 < 50 && xielv_right2 < 50)
            {
                xj = left_turn_down[1];  //||((xielv_1eft2<20&& xielv_right2>30))
                xk = right_turn_down[1]; //||((xielv_1eft2>30&& xielv_right2<20))
                xj = range_protect(xj, 2, 184);
                xk = range_protect(xk, 2, 184);

                for (unsigned int j = xj + 10; j <= xk - 10; j++)
                {
                    if (white_num_col[j] < white_num_col_min)
                    {
                        white_num_col_min = white_num_col[j];
                        white_num_col_min_line = j;
                    }
                }

                if (white_num_col_min_line <= (th_y + 15) && white_num_col_min_line >= (th_y - 15)) // 10
                {
                    if (white_num_col_line > (th_y + 15) || white_num_col_line < (th_y - 15))
                    {
                        cnt3++;
                    }
                    if (cnt3 > 1)
                    {
                        cnt3 = 0;
                    }
                }
            }

            if (!three_cross && !three_cross1 && !youhuihuan_flag && !lefthuihuan_flag)
            {
                ////std::cout<<"进入第二回环"<<std::endl;
                for (unsigned int j = 65; j >= 3; j--)
                {
                    ////std::cout << "\033[32m左上拐点检索\033[0m" << std::endl;
                    // 左上拐点
                    if (((j < (unsigned int)left_turn_down[0]) && ((left_line[j] - left_line[j + 3]) >= 10) && ((left_line[j] - left_line[j + 2]) >= 10) && ((left_line[j] - left_line[j + 1]) >= 10)) && Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0)
                    {

                        left_turn_up[0] = j - 3; // 数组里面没有第0行
                        left_turn_up[1] = left_line[j] - 3;
                        // 获得的上坐标先确定一下是不是比下坐标小，如果小则说明提前断掉，此时的“上拐点”为假.
                        // 如果比下坐标大则此时的“上拐点”为真.
                        if (left_turn_up[0] >= left_turn_down[0])
                        {
                            ////std::cout << "\033[32m找到左上拐点\033[0m" << std::endl;
                        }
                        else
                        {
                            ////std::cout << "\033[32m找左上拐点失败\033[0m" << std::endl;
                            break;
                        }
                    }
                }
                /***找右上拐点***********/
                for (unsigned int j = 65; j >= 3; j--)
                {
                    ////std::cout << "\033[32m右上拐点检索\033[0m" << std::endl;
                    if (((j < (unsigned int)right_turn_down[0]) && right_line[j + 3] - right_line[j] >= 10 && right_line[j + 2] - right_line[j] >= 10 && right_line[j + 1] - right_line[j] >= 10) && (Right_Add[j] == 0 && Right_Add[j - 1] == 0 && Right_Add[j - 2] == 0))
                    {
                        right_turn_up[0] = j - 3;
                        right_turn_up[1] = right_line[j - 3];
                        if (right_turn_up[0] >= right_turn_down[0])
                        {
                            ////std::cout << "\033[32m找到右上拐点\033[0m" << std::endl;
                        }
                        else
                        {
                            ////std::cout << "\033[32m找右上拐点失败\033[0m" << std::endl;
                            break;
                        }
                    }
                }
                if (right_turn_up[0] > (sousuojieshuhang) && right_turn_up[0] < right_turn_down[0] && right_turn_up[0] != 0)
                {
                    ////std::cout << "\033[31m这是红色文字\033[0m" << std::endl;
                    ////std::cout << "\033[32m找到右上拐点\033[0m" << std::endl;
                    findrightupguai = 1; // 表示找到右上拐点了
                }
                if (left_turn_up[0] > (sousuojieshuhang) && left_turn_up[0] < left_turn_down[0] && left_turn_up[0] != 0)
                {
                    ////std::cout << "\033[32m找到左上拐点\033[0m" << std::endl;
                    findleftupguai = 1; // 表示找到左上拐点了
                }

                /*********开始补线(找到左下拐点和左上拐点  或 找到右下拐点和右上拐点)*********/
                if ((findleftupguai == 1 && findleftdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 0) || (findleftupguai == 1 && findleftdownguai == 0))
                {
                    ////std::cout << "\033[32m开始补线\033[0m" << std::endl;
                    if (findleftupguai == 1 && findleftdownguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                    {
                        unsigned int start1 = left_turn_down[0] + 2;
                        if (start1 >= 68)
                            start1 = 68;

                        unsigned int start2 = left_turn_up[0];
                        if (start2 >= 68)
                            start2 = 68;

                        left_buxian(left_line[start1], start1, left_line[start2], start2);
                    }
                    else if (findleftupguai == 1 && findleftdownguai == 0)
                    {
                        unsigned int start1 = left_turn_up[0];
                        if (start1 >= 68)
                            start1 = 68;

                        left_buxian(2, 68, left_line[start1], start1);
                    }
                    // 并列关系
                    if (findrightdownguai == 1 && findrightupguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                    {
                        unsigned int start1 = right_turn_down[0] + 2;
                        if (start1 >= 68)
                            start1 = 68;

                        unsigned int start2 = right_turn_up[0];
                        if (start2 >= 68)
                            start2 = 68;
                        unsigned int end2 = right_turn_up[0];
                        right_buxian(right_line[start1], start1, right_line[end2], end2);
                    }
                    else if (findrightdownguai == 0 && findrightupguai == 1)
                    {
                        int start1 = right_turn_up[0];
                        if (start1 >= 68)
                            start1 = 68;

                        right_buxian(184, 68, right_line[start1], start1);
                    }
                }
            }
        }
    }
    if (!youhuihuan_flag && !lefthuihuan_flag && !three_cross1 && !three_cross && (!left_huan_num && !right_huan_num) && (l_start < 65 && r_start < 65 && abs(l_start - r_start) <= 12) && times >= 5)
    {
        for (unsigned int j = 68; j >= 1; j--)
        {
            // 左上拐点
            if (((j < (unsigned int)left_turn_down[0]) && ((left_line[j] - left_line[j + 3]) >= 10) && ((left_line[j] - left_line[j + 2]) >= 10) && ((left_line[j] - left_line[j + 1]) >= 10)) && Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0)
            {

                left_turn_up[0] = j - 2; // 数组里面没有第0行
                left_turn_up[1] = left_line[j] - 2;
                // 获得的上坐标先确定一下是不是比下坐标小，如果小则说明提前断掉，此时的“上拐点”为假.
                // 如果比下坐标大则此时的“上拐点”为真.
                if (left_turn_up[0] >= left_turn_down[0])
                {
                    ;
                }
                else
                    break;
            }
        }
        /***找右上拐点***********/
        for (unsigned int j = 68; j >= 1; j--)
        {
            if (((j < (unsigned int)right_turn_down[0]) && right_line[j + 3] - right_line[j] >= 20 && right_line[j + 2] - right_line[j] >= 20 && right_line[j + 1] - right_line[j] >= 20) && (Right_Add[j] == 0 && Right_Add[j - 1] == 0 && Right_Add[j - 2] == 0))
            {
                right_turn_up[0] = j - 2;
                right_turn_up[1] = right_line[j - 2];
                if (right_turn_up[0] >= right_turn_down[0])
                {
                    ;
                }
                else
                    break;
            }
        }
        if (right_turn_up[0] > (sousuojieshuhang) && right_turn_up[0] < right_turn_down[0] && right_turn_up[1] <= right_turn_down[1] && right_turn_up[0] != 0)
        {

            findrightupguai = 1; // 表示找到右上拐点了
        }
        if (left_turn_up[0] > (sousuojieshuhang) && left_turn_up[0] < left_turn_down[0] && left_turn_up[1] >= left_turn_down[1] && left_turn_up[0] != 0)
        {

            findleftupguai = 1; // 表示找到左上拐点了
        }
        /*********开始补线(找到左下拐点和左上拐点  或 找到右下拐点和右上拐点)*********/
        if ((findleftupguai == 1 && findleftdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 0) || (findleftupguai == 1 && findleftdownguai == 0))
        {

            if (findleftupguai == 1 && findleftdownguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
            {
                unsigned int start1 = left_turn_down[0] + 2;
                if (start1 >= 68)
                    start1 = 68;

                unsigned int start2 = left_turn_up[0];
                if (start2 >= 68)
                    start2 = 68;

                left_buxian(left_line[start1], start1, left_line[start2], start2);
            }
            else if (findleftupguai == 1 && findleftdownguai == 0)
            {
                unsigned int start1 = left_turn_up[0];
                if (start1 >= 68)
                    start1 = 68;

                left_buxian(2, 68, left_line[start1], start1);
            }
            // 并列关系
            if (findrightdownguai == 1 && findrightupguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
            {
                unsigned int start1 = right_turn_down[0] + 2;
                if (start1 >= 68)
                    start1 = 68;
                unsigned int end1 = right_turn_down[0] + 1;
                unsigned int start2 = right_turn_up[0];
                if (start2 >= 68)
                    start2 = 68;
                unsigned int end2 = right_turn_up[0];
                right_buxian(right_line[start1], start1, right_line[end2], end2);
            }
            else if (findrightdownguai == 0 && findrightupguai == 1)
            {
                unsigned int start1 = right_turn_up[0];
                if (start1 >= 68)
                    start1 = 68;
                unsigned int end1 = right_turn_up[0];
                right_buxian(184, 68, right_line[start1], start1);
            }
        }
    }
}
////中线修复逻辑
void Mid_Line_Repair(int count)
{
    for (unsigned int i = 68; i > count; i--)
    {
        //// 赛道宽度动态调整
        // Half_width_handao[i] = Half_width_handao2[i] + huan_bankuan;
        // Half_width[i] = Half_width2[i] + com_bankuan;
        {
            ////左边界贴左边缘（≤18）且右边界未贴右边缘（<184）
            if (Left_Line_New[i] <= 18 && Right_Line_New[i] < 184 && !three_cross && right_huan_num != 7 && left_huan_num != 7 && right_huan_num != 5 && left_huan_num != 5 && right_huan_num != 3 && left_huan_num != 3)
            {
                if (Right_Line_New[i] - Half_width[i] <= 18)
                    center[i] = 18;
                else if (right_huan_num == 8 || left_huan_num == 8 ||
                         right_huan_num == 9 || left_huan_num == 9 ||
                         right_huan_num == 1 || left_huan_num == 1 ||
                         right_huan_num == 2 || left_huan_num == 2 ||
                         youhuihuan_flag == 1 || lefthuihuan_flag == 1 || (huihuan_num > 18 && !poer_flag))
                {
                    center[i] = Right_Line_New[i] - Half_width_yuanshi[i]; //||huihuan_num>15||sousuojieshuhang<=3
                    // std::cout<<"循环1"<<std::endl;
                }
                else
                {
                    if (right_huan_num != 0 || left_huan_num != 0)
                        center[i] = Right_Line_New[i] - Half_width_handao[i];
                    else
                        center[i] = Right_Line_New[i] - Half_width[i];
                    // std::cout<<"循环2"<<std::endl;
                }
            }
            ////右边界贴右边缘（≥184）且左边界未贴左边缘（>18）
            else if (Left_Line_New[i] > 18 && Right_Line_New[i] >= 184 && !three_cross && right_huan_num != 7 && left_huan_num != 7 && right_huan_num != 5 && left_huan_num != 5 && right_huan_num != 3 && left_huan_num != 3)
            {
                if (Left_Line_New[i] + Half_width[i] >= 184)
                    center[i] = 184;
                else if (
                    right_huan_num == 8 || left_huan_num == 8 ||
                    right_huan_num == 9 || left_huan_num == 9 ||
                    right_huan_num == 1 || left_huan_num == 1 ||
                    right_huan_num == 2 || left_huan_num == 2 ||
                    youhuihuan_flag == 1 || lefthuihuan_flag == 1 || (huihuan_num > 18 && !poer_flag))
                    center[i] = Left_Line_New[i] + Half_width_yuanshi[i];
                else
                {
                    if (right_huan_num != 0 || left_huan_num != 0)
                        center[i] = Left_Line_New[i] + Half_width_handao[i];
                    else
                        center[i] = Left_Line_New[i] + Half_width[i];
                }
            }
            ////常规情况（左右边界均有效）
            else
                center[i] = (Left_Line_New[i] + Right_Line_New[i]) / 2; // 左右边界都是修复过的,得到中线
        }
        ////保护机制 限制中线在图像有效范围内
        center[i] = range_protect(center[i], 18, 184); // 限幅保护
    }

    /******************************************************************曲率计算**************************************************************/
    ////近端曲率统计（60行到count行）   统计中线方向变化次数，差值越大说明弯道越急
    for (unsigned int i = 60; i > count; i--)
    {

        if ((center[y] - center[y + 1]) > 0)
            qulv_jinduan_right++;
        else if ((center[y] - center[y + 1]) < 0)
            qulv_jinduan_left++;
    }
    //  qvlv_quanju=abs(qvlv_quanju_right-qvlv_quanju_left);//曲率全局qvlv_quanju qulv_jinduan qulv_yuandaun
    qulv_jinduan = abs(qulv_jinduan_right - qulv_jinduan_left); // 曲率近端
    //  qulv_yuandaun=abs(qulv_yuandaun_right-qulv_yuandaun_left);//曲率远端

    //// 中线斜率拟合
    regression(0, sousuojieshuhang + 3, 65);
    k_center = parameterB;
    for (unsigned int i = 68; i > sousuojieshuhang; i--)
    {
        //         image_use[i][left[i]+3] =1;
        //          image_use[i][right[i]-3] =2;
        image_use[i][center[i]] = 0;
        image_use[i][Left_Line_New[i] + 3] = 0;
        image_use[i][Right_Line_New[i] - 3] = 0;
        //     image_use[i][center_th[i]] =0;
    }
}

float Point_Weight(void)
{
    char i;
    Sum = 0;
    Weight_Count = 0;

    ////根据速度
    if ((left_num + right_num) / 2 < 340)
        quanzhi_num = 26;
    else if ((left_num + right_num) / 2 < 360)
        quanzhi_num = 25;
    else if ((left_num + right_num) / 2 < 380)
        quanzhi_num = 23;
    else if ((left_num + right_num) / 2 < 400)
        quanzhi_num = 21;
    else if ((left_num + right_num) / 2 < 420)
        quanzhi_num = 19;
    else if ((left_num + right_num) / 2 < 440)
        quanzhi_num = 18;

    ////圆环情况
    if (left_huan_num != 0 || right_huan_num != 0)
    {
        if ((left_num + right_num) / 2 < 340)
            quanzhi_num = 23;
        else if ((left_num + right_num) / 2 < 360)
            quanzhi_num = 22;
        else if ((left_num + right_num) / 2 < 380)
            quanzhi_num = 21;
        else if ((left_num + right_num) / 2 < 400)
            quanzhi_num = 20;
    }

    ////注意实验
    quanzhi_num = 25;
    ////根据速度大小quanzhi_num 修改动态权值
    for (int i = quanzhi_num; i < quanzhi_num + 26; i++)
    {
        weight_jubu[i] = Weight_jtai[i - quanzhi_num];
    }

    // if(park_flag==1&&j_flag==1&&(time_flag-time_flag1)<8)
    if (park_flag == 1 && star_lineflag == 1)
    {
        // j_flag=1;
        for (int i = 0; i < 70; i++)
            weight_jubu[i] = Weight_park[i];
    }

    if (park_flag == 1 && star_lineflag == 1)
    {
        for (i = 68; i >= 1; i--) // 使用加权平均
        {

            Sum += center[i] * weight_jubu[i];
            Weight_Count += weight_jubu[i];
        }
        if (Weight_Count)
            Points = Sum / Weight_Count;
        if (Points > 184)
            Points = 184;
        if (Points < 18)
            Points = 18;
    }

    else
    {
        if (sousuojieshuhang >= 15)
        {
            for (i = 68; i >= sousuojieshuhang; i--) // 使用加权平均
            {

                Sum += center[i] * weight_jubu[i];
                Weight_Count += weight_jubu[i];
            }
            if (Weight_Count)
                Points = Sum / Weight_Count; ////Points为动态计算后的偏移 范围18~184
            if (Points > 184)
                Points = 184;
            if (Points < 18)
                Points = 18;
        }
        else
        {
            for (i = 68; i >= 15; i--) // 使用加权平均
            {

                Sum += center[i] * weight_jubu[i];
                Weight_Count += weight_jubu[i];
            }
            if (Weight_Count)
                Points = Sum / Weight_Count;
            if (Points > 184)
                Points = 184;
            if (Points < 18)
                Points = 18;
        }
    }
    Point_last3 = Point_last2;
    Point_last2 = Point_last1;
    Point_last1 = Points;

    Points = Point_last1 * 0.7 + Point_last2 * 0.1 + Point_last3 * 0.05; ////类平滑滤波

    /***** 使用最远行数据和目标点作为前瞻 *****/
    if (sousuojieshuhang < 30)
    {
        Point_Mid = center[30];
    }
    else
    {
        Point_Mid = center[sousuojieshuhang + 1];
    }

    Foresight = 0.8 * (Point_Mid - 91) + 0.2 * (Points - 91);

    return Points;
}

void jieyahuansuan()
{
    unsigned int y = 0;
    for (y = 69; y > 10; y--)
    {
        image_use[y][left_line[y + 4]] = 0;  // 1
        image_use[y][right_line[y - 4]] = 0; // 3
        image_use[y][center[y]] = 0;         // 2
    }
}

int range_protect(int duty, int min, int max) // 限幅保护
{
    if (duty >= max)
    {
        return max;
    }
    if (duty <= min)
    {
        return min;
    }
    else
    {
        return duty;
    }
}
long long int range_protect2(long long int duty, long long int min, long long int max) // 限幅保护
{
    if (duty >= max)
    {
        return max;
    }
    if (duty <= min)
    {
        return min;
    }
    else
    {
        return duty;
    }
}

void star_line_judg() // 斑马线检测
{
    sd_fast = 33;
    clc_sd = 7;
    baihei_flag = 0;
    heibai_flag = 0;
    for (kk = left_line[sd_fast - 4]; kk <= right_line[sd_fast - 4]; kk++)
    {
        if (image_use[sd_fast][kk] == 255)
        {
            bai_flag = 1;
            s1 = kk;
        }
        else if (bai_flag && image_use[sd_fast][kk] == 0)
        {
            s2 = kk;
            if ((s2 - s1) == 1 || (s2 - s1) == 2)
            {
                baihei_flag++;
                bai_flag = 0;
            }
            else
                bai_flag = 0;
        }

        if (image_use[sd_fast][kk] == 0)
        {
            hei_flag = 1;
            s3 = kk;
        }
        else if (hei_flag && image_use[sd_fast][kk] == 255)
        {
            s4 = kk;
            if ((s4 - s3) == 1 || (s4 - s3) == 2)
            {
                heibai_flag++;
                hei_flag = 0;
            }
            else
                hei_flag = 0;
        }
    }
    if ((baihei_flag >= clc_sd && heibai_flag >= clc_sd)) // 442
        star_lineflag = 1;                                // 改成1停车
}

void parkcar()
{
    //   park_flag = 1;
    leijia_flag = 1;
    if (star_lineflag)
    {
        if (c_l == 1)
        {
            //  pwm_duty(S_MOTOR_PIN,stree_max);     //左边
        }
        else if (c_l == 2)
        {

            //   pwm_duty(S_MOTOR_PIN,stree_min);    //右边
        }
        else if (c_l == 3)
        {
            //  pwm_duty(S_MOTOR_PIN,stree_center);    //中间
        }
    }
}

////计算自适应阈值
unsigned int my_adapt_threshold(unsigned int *image, long int col, long int row) // 注意计算阈值的一定要是原图像
{
#define GrayScale 256
    long int width = col;
    long int height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height / 4;
    unsigned int threshold = 0;
    unsigned int *data = image; // 指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    unsigned long int gray_sum = 0;
    // 统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i += 2)
    {
        for (j = 0; j < width; j += 2)
        {
            pixelCount[(int)data[i * width + j]]++; // 将当前的点的像素值作为计数数组的下标
            gray_sum += (int)data[i * width + j];   // 灰度值总和
        }
    }

    // 计算每个像素值的点在整幅图像中的比例

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }

    // 遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;

    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)
    {

        w0 += pixelPro[j];        // 背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j]; // 背景部分 每个灰度值的点的比例 *灰度值

        w1 = 1 - w0;
        u1tmp = gray_sum / pixelSum - u0tmp;

        u0 = u0tmp / w0;   // 背景平均灰度
        u1 = u1tmp / w1;   // 前景平均灰度
        u = u0tmp + u1tmp; // 全局平均灰度
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }
    }

    return threshold;
}
////采用 最小二乘法 拟合直线
////斜率 parameterB     截距 parameterA
void regression(int type, int startline, int endline)
{
    int i = 0;
    int sumlines = endline - startline;
    int sumX = 0;
    int sumY = 0;
    float averageX = 0;
    float averageY = 0;
    float sumUp = 0;
    float sumDown = 0;
    if (type == 0) // 拟合中线
    {
        for (i = startline; i < endline; i++)
        {
            sumX += i;
            sumY += center_th[i];
        }
        if (sumlines != 0)
        {
            averageX = sumX / sumlines; // x的平均值
            averageY = sumY / sumlines; // y的平均值
        }
        else
        {
            averageX = 0; // x的平均值
            averageY = 0; // y的平均值
        }
        for (i = startline; i < endline; i++)
        {
            sumUp += (center_th[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0)
            parameterB = 0;
        else
            parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;
    }
    else if (type == 1) // 拟合左线
    {
        for (i = startline; i < endline; i++)
        {
            sumX += i;
            sumY += left_line[i];
        }
        if (sumlines == 0)
            sumlines = 1;
        averageX = sumX / sumlines; // x的平均值
        averageY = sumY / sumlines; // y的平均值
        for (i = startline; i < endline; i++)
        {

            sumUp += (left_line[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0)
            parameterB = 0;
        else
            parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;
    }
    else if (type == 2) // 拟合右线
    {
        for (i = startline; i < endline; i++)
        {
            sumX += i;
            sumY += right_line[i];
        }
        if (sumlines == 0)
            sumlines = 1;
        averageX = sumX / sumlines; // x的平均值
        averageY = sumY / sumlines; // y的平均值
        for (i = startline; i < endline; i++)
        {
            sumUp += (right_line[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0)
            parameterB = 0;
        else
            parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;
    }
}

void check_starting_line()
{
    // int[] black_nums_stack = new int[20];
    times2 = 0;                             ////times2为指定行斑马线数
    for (unsigned int y = 21; y <= 29; y++) // 27  32//斑马线识别（如果识别迟了就调小点，识别早了就大点）
    {
        black_blocks = 0;
        cursor = 0;                                                            // 指向栈顶的游标
        for (unsigned int x = left_line[35] + 5; x <= right_line[35] - 5; x++) ////+-5防止边缘干扰
        {
            if (image_use[y][x] == 0)
            {
                if (cursor >= 20)
                {
                    // 当黑色元素超过栈长度的操作   break;
                }
                else
                {
                    cursor++;
                }
            }
            else
            {
                if (cursor >= 2 && cursor <= 8)
                {
                    black_blocks++;
                    cursor = 0;
                }
                else
                {
                    cursor = 0;
                }
            }
        } ////black_blocks记录总的斑马线块数，cursor记录当前斑马线块的长度
        if (black_blocks >= 3 && black_blocks <= 10)
            times2++;
    }
    if (times2 >= 3 && times2 <= 9) // 6
    {
        star_lineflag = 1; ////起跑线标志位
    }
    else
    {
        star_lineflag = 0;
    }
}
