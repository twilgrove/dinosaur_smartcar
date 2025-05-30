
#include "image_deal.h"
#include "stdio.h"
#include <cmath>
#define CAMERA_H 70
#define CAMERA_W 188
float BlackThres = 160.0; // 黑白阈值
int kk, bai_flag = 0, hei_flag = 0, baihei_flag = 0, heibai_flag = 0, width_heibai = 0;
int s1, s2, s3, s4;
int twolines_trend = 2;
int uart_buf[10];
extern long int time_flag; /////////////
long int time_flag1;
int Point_last1 = 95, Point_last2 = 95, Point_last3 = 95;
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
        2, 3, 4, 6, 8, 11,                      // 图像最远端10——20行权重
        15, 17, 18, 21, 20, 19, 18, 17, 16, 15, // 图像最远端20——30行权重
        13, 12, 11, 10, 9, 8, 7, 6, 5, 4,       // 图像最远端30——40行权重
                                          // 图像最远端50——60行权重
}; // 69

const unsigned int Weight_huandao[70] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端60——70行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端   0——10行权重
        1, 1, 1, 1, 2, 3, 4, 6, 8, 11,          // 图像最远端10——20行权重
        15, 17, 18, 21, 20, 19, 18, 17, 16, 15, // 图像最远端20——30行权重
        13, 12, 11, 10, 9, 8, 7, 6, 5, 4,       // 图像最远端30——40行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端40——50行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,           // 图像最远端50——60行权重

}; // 69
const unsigned int Weight_huihuan[70] =
    {
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1, // 图像最远端60——70行权重
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1, // 图像最远端   0——10行权重
        1,
        1,
        1,
        1,
        2,
        3,
        4,
        6,
        8,
        11, // 图像最远端10——20行权重
        15,
        17,
        18,
        21,
        20,
        19,
        18,
        17,
        16,
        15, // 图像最远端20——30行权重
        13,
        12,
        11,
        10,
        9,
        8,
        7,
        6,
        5,
        4, // 图像最远端30——40行权重
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1, // 图像最远端40——50行权重
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
}; // 69
int huihuan_num = 0, zhidao_num = 0, huandao_7 = 0;
// const unsigned int Weight_huihuan_flag[70]=
//{
//         01,01,01,01,01,01,01,01,01,01,
//         1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
//         1,1,1,1, 1,1,1,1,1,1,
//         1,1,1,1,1,1,1,1,1,1,
//         2,3,4,6,8,11,15,17,18,21,
//         20,19,18,17,16,15,13,12,11,10,
//         9,8,7,6,5,4,
//
//
// };

unsigned int weight_jubu[70] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 图像最远端60——70行权重
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 图像最远端   0——10行权重
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
unsigned int Foresight_Left = 0;////左侧最小赛道宽度
unsigned int Foresight_Right = 186;////右侧最小赛道宽度
int Point_Mid = 0, Points = 0, Foresight = 0;
unsigned char image_use[70][188];
int whitenum = 0;
//

unsigned int Half_width[70] =
{
    00, 00, 00, 00, 00, 00, 00, 00, 00, 15+30,
    16+30, 16+30, 17+30, 46+30, 47+30, 48+30, 49+30, 50+30, 51+30, 52+30,
    54+30, 55+30, 55+30, 55+30, 56+30, 56+30, 57+30, 57+30, 58+30, 58+30,
    59+30, 59+30, 60+30, 60+30, 62+30, 62+30, 63+30, 64+30, 65+30, 65+30,
    66+30, 66+30, 67+30, 67+30, 68+30, 68+30, 69+30, 70+30, 70+30, 70+30,
    70+30, 71+30, 71+30, 71+30, 72+30, 72+30, 72+30, 73+30, 73+30, 73+30,
    74+30, 74+30, 74+30, 75+30, 75+30, 76+30, 76+30, 77+30, 77+30, 78+30
};

unsigned int const Half_width2[70] =
{
    00, 00, 00, 00, 00, 00, 00, 00, 00, 15+30,
    16+30, 16+30, 17+30, 46+30, 47+30, 48+30, 49+30, 50+30, 51+30, 52+30,
    54+30, 55+30, 55+30, 55+30, 56+30, 56+30, 57+30, 57+30, 58+30, 58+30,
    59+30, 59+30, 60+30, 60+30, 62+30, 62+30, 63+30, 64+30, 65+30, 65+30,
    66+30, 66+30, 67+30, 67+30, 68+30, 68+30, 69+30, 70+30, 70+30, 70+30,
    70+30, 71+30, 71+30, 71+30, 72+30, 72+30, 72+30, 73+30, 73+30, 73+30,
    74+30, 74+30, 74+30, 75+30, 75+30, 76+30, 76+30, 77+30, 77+30, 78+30
};

// unsigned int  const Half_width_handao[70]=  //time_flag
//         {
//                 00,00,00,00,00,00,00,00,00,7,
//                 8,8,9,38,39,40,41,42,43,44,
//                 46,47,47,47,48,48,49,49,50,50,
//                 51,51,52,52,54,54,55,59,60,60,
//                 61,61,62,62,63,63,64,65,65,65,
//                 65,66,66,66,67,67,67,68,68,68,
//                 69,69,69,70,70,71,71,72,72,73
//         };

unsigned int Half_width_handao[70] = // time_flag
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 10+30,
    11+30, 11+30, 12+30, 14+30, 15+30, 16+30, 17+30, 18+30, 19+30, 20+30,
    28+30, 28+30, 29+30, 29+30, 30+30, 31+30, 31+30, 32+30, 32+30, 33+30,
    48+30, 51+30, 52+30, 52+30, 54+30, 54+30, 55+30, 59+30, 60+30, 60+30,
    61+30, 61+30, 62+30, 62+30, 63+30, 63+30, 64+30, 65+30, 65+30, 65+30,
    65+30, 66+30, 66+30, 66+30, 67+30, 67+30, 67+30, 68+30, 68+30, 68+30,
    69+30, 69+30, 69+30, 70+30, 70+30, 71+30, 71+30, 72+30, 72+30, 73+30
};

unsigned int const Half_width_handao2[70] = // time_flag
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 10+30,
    11+30, 11+30, 12+30, 14+30, 15+30, 16+30, 17+30, 18+30, 19+30, 20+30,
    28+30, 28+30, 29+30, 29+30, 30+30, 31+30, 31+30, 32+30, 32+30, 33+30,
    48+30, 51+30, 52+30, 52+30, 54+30, 54+30, 55+30, 59+30, 60+30, 60+30,
    61+30, 61+30, 62+30, 62+30, 63+30, 63+30, 64+30, 65+30, 65+30, 65+30,
    65+30, 66+30, 66+30, 66+30, 67+30, 67+30, 67+30, 68+30, 68+30, 68+30,
    69+30, 69+30, 69+30, 70+30, 70+30, 71+30, 71+30, 72+30, 72+30, 73+30
};

// unsigned int  const Half_width_handao[70]=  //time_flag   椒丝花1111  大环适配
//         {
//                 00,00,00,00,00,00,00,00,00,10,
//                 11,11,12,14,15,16,17,18,19,20,
//                 28,28,29,29,30,31,31,32,32,33,
//                 48,51,52,52,54,54,55,59,60,60,
//                 61,61,62,62,63,63,64,65,65,65,
//                 65,66,66,66,67,67,67,68,68,68,
//                 69,69,69,70,70,71,71,72,72,73
//         };
// unsigned int  const Half_width_handao[70]=
//         {
//                 00,00,00,00,00,00,00,00,00,10,
//                 13,13,14,16,17,18,19,20,21,22,
//                 30,30,31,31,32,33,33,34,34,38,
//                 50,53,54,54,56,56,57,61,62,62,
//                 63,63,63,64,65,65,66,67,67,67,
//                 67,68,86,68,69,9,69,70,70,70,
//                 71,71,71,72,72,3,73,74,74,75
//         };


////直接处理过程中使用的赛道半宽
unsigned int Half_width_yuanshi[70] ={
    00, 00, 00, 00, 00, 17, 18, 19, 20, 22,
    22, 23, 25, 26, 27, 28, 29, 30, 31, 33,
    34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 
    44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 
    54, 56, 57, 58, 59, 60, 61, 62, 63, 64,
    65, 66, 67, 69, 70, 71, 72, 74, 75, 76, 
    77, 78, 79, 80, 81, 82, 83, 84, 82, 82,
};



// unsigned int  const Half_width[70]=
//{
// 00,00,00,00,00,00,00,00,00,10,
// 11,11,12,14,15,16,17,18,19,20,
// 21,22,23,24,25,25,26,37,43,43,
// 44,45,45,46,47,47,48,49,50,50,
// 51,52,53,54,55,55,56,57,58,59,
// 60,61,62,64,64,65,67,68,68,69,
// 70,73,73,73,74,74,75,76,77,78
// };
int c = 0;
int huan2_flag = 0;
int star_lineflag = 0, star_lineflag2 = 0;
int y = 0;
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
/*****************大津法参数*********************/
float bin_float[256]; // 灰度比例直方图
int size = 70 * 186;
float u = 0; // 全图平均灰度
float w0 = 0;
float u0 = 0; // 前景灰度
int Bin_Array[256];
int i;
float gray_hh = 0; // 前景灰度和
float var = 0;     // 方差
float maxvar = 0;  // 最大方差
float maxgray = 0; // 最大灰度占比
float maxbin = 0;

struct size_point
{
    int x0;
    int y0;
    int x1;
    int y1;
};

// struct size_point ostu_point[3]={
//         {0,0,15,69},
//         {16,0,160,69},
//         {161,0,186,69},
// };
struct size_point ostu_point[3] = {
    {0, 0, 40, 69},
    {41, 0, 135, 69},
    {136, 0, 186, 69},
};
/*****************大津法参数end*********************/

/*****************大津end*********************/

/***八邻域*****/
struct size_point2
{
    int x0;
    int y0;
};

struct size_point2 stack_seed[6000]; // 栈
long int stack_top = 0;
unsigned char (*p_Pixels)[188] = &image_use[0];
unsigned int Ostu_Threshold = 0;
void pull_stack(unsigned int x, unsigned int y) // 入栈
{
    *(*(p_Pixels + y) + x) = 255;
    stack_seed[stack_top].x0 = x;
    stack_seed[stack_top].y0 = y;
    stack_top++;
}
struct size_point2 push_stack() // 出栈
{
    stack_seed[stack_top].y0 = 0;
    stack_seed[stack_top].x0 = 0;
    return stack_seed[--stack_top];
}

int panbianjie(int x,int y)
{
    if (x + y == 0)
    {
        return 0;
    }
    return (int)((abs(x - y) * 100 / (x + y)) + 0.5f);
}

struct size_point2 connects[8] = { // 八领域扫点
    //{-1,-1},
    //{0,-1},
    //{1,-1},
    {1, 0},
    //{1,1},
    {0, 1},
    //{-1,1},
    {-1, 0}};

void SignalProcess_grayfine_fill(void)
{
//     int j, px, py;
//     struct size_point2 center_seed;
//     unsigned int(*p_image)[188] = &mt9v03x_image[0];
//     p_Pixels = &image_use[0];
//     stack_top = 0;
//     for (i = 0; i <= 69; i++)
//     {
//         for (j = 0; j <= 185; j++)
//         {
//             *(*(p_Pixels + i) + j) = 0;
//         }
//     }
//     Ostu_Threshold = threshold1;

//     for (i = 0; i < 186; i++)
//     {
//         if (Ostu_Threshold - *(*(p_image + 0) + i + 1) < 5)
//             pull_stack((unsigned int)i, 0);
//     }
//     while (stack_top != 0)
//     {
//         center_seed = push_stack();
//         px = center_seed.x0 + connects[0].x0;
//         py = center_seed.y0 + connects[0].y0;
//         if (*(*(p_Pixels + py) + px) == 1 || px < 0 || py < 0 || px >= 186 || py >= 70)
//         {
//         }
//         else
//         {
//             if (abs(*(*(p_image + py) + px + 1) - *(*(p_image + center_seed.y0) + center_seed.x0 + 1)) < 8 && Ostu_Threshold - *(*(p_image + py) + px + 1) < 5)
//             {
//                 pull_stack((unsigned int)px, (unsigned int)py);
//             }
//         }

//         px = center_seed.x0 + connects[1].x0;
//         py = center_seed.y0 + connects[1].y0;
//         if (*(*(p_Pixels + py) + px) == 1 || px < 0 || py < 0 || px >= 186 || py >= 70)
//         {
//         }
//         else
//         {
//             if (abs(*(*(p_image + py) + px + 1) - *(*(p_image + center_seed.y0) + center_seed.x0 + 1)) < 8 && Ostu_Threshold - *(*(p_image + py) + px + 1) < 5)
//             {
//                 pull_stack((unsigned int)px, (unsigned int)py);
//             }
//         }

//         px = center_seed.x0 + connects[2].x0;
//         py = center_seed.y0 + connects[2].y0;
//         if (*(*(p_Pixels + py) + px) == 1 || px < 0 || py < 0 || px >= 186 || py >= 70)
//         {
//         }
//         else
//         {
//             if (abs(*(*(p_image + py) + px + 1) - *(*(p_image + center_seed.y0) + center_seed.x0 + 1)) < 8 && Ostu_Threshold - *(*(p_image + py) + px + 1) < 5)
//             {
//                 pull_stack((unsigned int)px, (unsigned int)py);
//             }
//         }
//     }
}
/***八邻域*****/

void bu_breakhang(int c1, int c2, unsigned int j)
{

    int k = center[c2] - center[c1];

    if (j > 40)
    {
        if (k > 1) // 入左弯
        {
            for (unsigned int i = j; i >= 11; i--)
            {
                center[i] = 2;
            }
        }
        else if (k < 1) // 入右弯
        {
            for (unsigned int i = j; i >= 11; i--)
            {
                center[i] = 184;
            }
        }
        else //          直到置0
        {
            for (unsigned int i = j; i >= 1; i--)
            {
                center[i] = 93;
            }
        }
    }
}

void advanced_regression(int type, int startline1, int endline1, int startline2, int endline2)
{
    int i = 0;
    int sumlines1 = endline1 - startline1;
    int sumlines2 = endline2 - startline2;
    int sumX = 0;
    int sumY = 0;
    float averageX = 0;
    float averageY = 0;
    float sumUp = 0;
    float sumDown = 0;
    if (type == 0) // 拟合中线
    {
        /**计算sumX sumY**/
        for (i = startline1; i < endline1; i++)
        {
            sumX += i;
            sumY += center_th[i];
        }
        for (i = startline2; i < endline2; i++)
        {
            sumX += i;
            sumY += center_th[i];
        }
        averageX = sumX / (sumlines1 + sumlines2); // x的平均值
        averageY = sumY / (sumlines1 + sumlines2); // y的平均值
        for (i = startline1; i < endline1; i++)
        {
            sumUp += (center_th[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i < endline2; i++)
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
        /**计算sumX sumY**/
        for (i = startline1; i < endline1; i++)
        {
            sumX += i;
            sumY += left_line[i];
        }
        for (i = startline2; i < endline2; i++)
        {
            sumX += i;
            sumY += left_line[i];
        }
        averageX = sumX / (sumlines1 + sumlines2); // x的平均值
        averageY = sumY / (sumlines1 + sumlines2); // y的平均值
        for (i = startline1; i < endline1; i++)
        {
            sumUp += (left_line[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i < endline2; i++)
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
        /**计算sumX sumY**/
        for (i = startline1; i < endline1; i++)
        {
            sumX += i;
            sumY += right_line[i];
        }
        for (i = startline2; i < endline2; i++)
        {
            sumX += i;
            sumY += right_line[i];
        }
        averageX = sumX / (sumlines1 + sumlines2); // x的平均值
        averageY = sumY / (sumlines1 + sumlines2); // y的平均值
        for (i = startline1; i < endline1; i++)
        {
            sumUp += (right_line[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i < endline2; i++)
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
void find_leftdown_point(int start_point,int end_point,int RoadName)
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
                l_start = i;////l_start首次丢线行
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

void Center_line_deal() // 中线处理
{
    k_center = 0;
    qvlv_quanju_right = qvlv_quanju_left = qulv_jinduan_right = qulv_jinduan_left = qulv_yuandaun_right = qulv_yuandaun_left = 0;
    k_left = 0;
    k_right = 0;
    Width_Min = 100;
    // th_y=0;
    whitenum = 0;
    right_turn_down[0] = 69;
    left_turn_down[0] = 69;
    right_turn_down[1] = 187;
    left_turn_down[1] = 1;
    // right_turn_up[0]=0;
    //  left_turn_up[0]=0;
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
    for (int ql = 0; ql <= 69; ql++) // 清零函数
    {
        left_line[ql] = 2;////存储左边界线的列坐标
        Left_Add[ql] = 0;////标记左边界是否需要基础补线
        Left_Add2[ql] = 0;////标记左边界是否需要高级补线（动态斜率补线标志）
        Left_Line_New[ql] = 2;////存储修复后的左边界线（最终使用的左边界）
    }
    for (int ql = 0; ql <= 187; ql++) // 清零函数guaidian
    {
        white_num_col[ql] = 0;////白列白点数
    }
    for (int ql = 0; ql <= 69; ql++)
    {
        right_line[ql] = 184;////存储右边界线的列坐标
        Right_Line_New[ql] = 184;////存储修复后的右边界线（最终使用的右边界）
        center[ql] = 89;//每行中线点
        Right_Add[ql] = 0;////标记右边界是否需要基础补线
        Right_Add2[ql] = 0;////标记右边界是否需要高级补线（动态斜率补线标志）
    }
    unsigned int x = 0, y = 0; // 设x为行，y为列
    unsigned int temp = 0;
    ////////////////////////////扫描最底下3行/////////////////////////
    ////寻找最长白列
    if (left_huan_num == 3) // right_huan_num==6||
    {
        for (x = 184; x > 101; x--)
        {
            for (y = 69; y >= 0; y--)////更新white_num_col[x]
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
                white_num_col_max = white_num_col[x];////white_num_col_max最大白列白点数
                white_num_col_line = x;////white_num_col_line最长白列
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
        else if (park_flag == 0)
        {

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
        for (x = white_num_col_line; x <= 184; x++) // 中间向右找跳变
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
        for (x = white_num_col_line; x >= 1; x--) // 中间向左找跳变
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
        Width[y] = abs(right_line[y] - left_line[y]);////Width赛道宽度

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
                    Left_Add2[y] = 1;////更新Left_Add2[]
                }
            }
            if (Right_Add2[y + 1])
            {
                if (right_line[y] > Right_Line_New2[y + 1] + 1)
                {
                    Right_Add2[y] = 1;////更新Right_Add2[]
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

        ////高级补线 斜率
        ////左补线
        if (Left_Add2[y]) // 左边需要补线
        {

            if (y < 65)//因为有下面+6限制
            {
                if (!Left_Add_Start) // 如果还没有记录开始补线位置
                {

                    Left_Add_Start = y; // 记录左边界补线开始位置
                }

                Add_Slope = 1.0 * (left_line[Left_Add_Start + 6] - left_line[Left_Add_Start + 1]) / 4; // 计算能识别的前几行图像斜率

                if (Add_Slope > 0) // 限幅
                {
                    Add_Slope = 0;
                }
                temp = (char)((y - (Left_Add_Start + 1)) * Add_Slope + left_line[Left_Add_Start + 1]); // 通过斜率推算补线的位置
                Left_Last_Slope = Add_Slope;                                                           // 更新上次左边界斜率

                Left_Line_New2[y] = range_protect(temp, 2, 184); // 不直接修改边界，只保存在补线数组里
            }
            /* 第一次补线，只记录，不在图像上显示 */
            //
        }
        ////右补线
        if (Right_Add2[y]) // 右边需要补线
        {
            if (y < 65)
            {
                if (!Right_Add_Start) // 如果还没有记录开始补线位置
                {

                    Right_Add_Start = y; // 记录左边界补线开始位置
                }

                Add_Slope = 1.0 * (right_line[Right_Add_Start + 6] - right_line[Right_Add_Start + 1]) / 4; // 计算能识别的前几行图像斜率

                if (Add_Slope < 0) // 限幅
                {
                    Add_Slope = 0;
                }
                temp_r = (char)((y - (Right_Add_Start + 1)) * Add_Slope + right_line[Right_Add_Start + 1]); // 通过斜率推算补线的位置
                Right_Last_Slope = Add_Slope;                                                               // 更细上次右边界斜率

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

    huihuan_num = 0;//纵向较大白列>66的数目
    huandao_7 = 0;//>68
    zhidao_num = 0;//>41
    ////计算纵向较大白列huihuan_num的数目
    for (x = right_line[68]; x >= left_line[68]; x--)
    {
        if (white_num_col[x] > 66)
            huihuan_num++;
    }
    
    ////计算纵向大白列zhidao_num和小白列huandao_7的数目
    for (x = 118; x >= 80; x--)
    {
        if (white_num_col[x] > 68)
            zhidao_num++;
        if (white_num_col[x] > 41)
            huandao_7++;
    }

    //         curvity_point1 = (unsigned int)((r_start + sousuojieshuhang) / 2);      //中点
    //
    //                if (sousuojieshuhang >=60)
    //                {
    //                  curvity_point2 = (unsigned int)(sousuojieshuhang + 1);
    //                }
    //                else
    //                {
    //                  curvity_point2 = (unsigned int)(sousuojieshuhang+1);
    //                }
    //         curvity_right = process_curvity(right_line[r_start], r_start, right_line[curvity_point1], curvity_point1, right_line[curvity_point2], curvity_point2);
    //
    //         curvity_point1 = (unsigned int)((l_start + sousuojieshuhang+1) / 2);      //中点
    //
    //                if (sousuojieshuhang >=60)
    //                {
    //                  curvity_point2 = (unsigned int)(sousuojieshuhang + 1);
    //                }
    //                else
    //                {
    //                  curvity_point2 = (unsigned int)(sousuojieshuhang+1);
    //                }
    //         curvity_left = process_curvity(left_line[l_start], l_start, left_line[curvity_point1], curvity_point1, left_line[curvity_point2], curvity_point2);

    /************十字处理**************/
    if (l_start >= 55 || r_start >= 55)
    {
        find_leftdown_point(67, 15, 1); // 1是十字////left_turn_down[0]存储左拐点Y  left_turn_down[1]存储左拐点X
        find_rightdown_point(67, 15, 1);
    }
    // find_leftmiddle_point(65,20);
    //   if(flag_find_huan_leftmiddle_point)
    //  {
    //   right_buxian(right_turn_middle[1],right_turn_middle[0],160,68);
    //    flag_find_huan_leftmiddle_point=0;
    //    }

    regression(1, 20, 68);
    k_left = parameterB;////左边界全局斜率拟合

    regression(2, 20, 68);////右边界全局斜率拟合
    k_right = parameterB;

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
            twolines_trend = 1;/////左右边界趋势相反（十字特征）
        else
            twolines_trend = 0;////趋势冲突

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

        //                                     if(findrightdownguai&&right_turn_down[0]<55&&right_turn_down[0]<left_turn_down[0])
        //                                         regression(0,right_turn_down[0]+2,68);
        //                                  else if(findleftdownguai&&left_turn_down[0]<55&&right_turn_down[0]>left_turn_down[0])
        //                                      regression(0,left_turn_down[0]+2,68);
        //                                  else
        regression(0, 58, 68);////近端中线拟合
        ////通过近端中线拟合结果扩展到全局
        for (int j = (unsigned int)68; j >= 1; j--)
        {
            int jicun = (int)(parameterB * j + parameterA);
            if (jicun >= 185)
                jicun = 185;
            else if (jicun <= 0)
                jicun = 0;
            center_th[j] = (unsigned int)jicun;
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
                    else if (!three_cross && right_turn_down[0] < 60)////?
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
                            if (park_flag != 2 && l_start >= 65 && r_start >= 65 && !poer_flag && !por_cnt) //
                            {
                                three_cross = 1;
                                //                                 uart_putchar(WIRELESS_UART, '6');
                                //                                 uart_putchar(WIRELESS_UART, '1');
                            }
                        }
                    }
                }

                if (!three_cross && !three_cross1 && !youhuihuan_flag && !lefthuihuan_flag)
                {

                    for (unsigned int j = 68; j >= 1; j--)
                    {
                        // 左上拐点
                        if (((j < (unsigned int)left_turn_down[0]) && ((left_line[j] - left_line[j + 3]) >= 10) && ((left_line[j] - left_line[j + 2]) >= 10) && ((left_line[j] - left_line[j + 1]) >= 10)) && Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0)
                        {

                            left_turn_up[0] = j - 3; // 数组里面没有第0行
                            left_turn_up[1] = left_line[j] - 3;
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
                            right_turn_up[0] = j - 3;
                            right_turn_up[1] = right_line[j - 3];
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
    }

    if ((!left_huan_num && !right_huan_num) && (three_cross1 || three_cross) && !youhuihuan_flag && !lefthuihuan_flag) //
    {

        if (park_flag == 1)
        {

            if (sousuojieshuhang <= 15) //&&(white_num_col_min_line<165)
            {
                if (findrightdownguai) // 第二圈
                    right_buxian(white_num_col_min_line, 68 - white_num_col[white_num_col_min_line], right_turn_down[1], right_turn_down[0]);
                else
                    right_buxian(white_num_col_min_line, 68 - white_num_col[white_num_col_min_line], right_line[68], 68);
                for (int ql = 68 - white_num_col[white_num_col_min_line] + 2; ql >= 10; ql--)
                {
                    Left_Line_New[ql] = 5;
                    Right_Line_New[ql] = 150;
                }
                sousuojieshuhang = 15;
            }
            else
            {
                right_buxian(80, 5, right_line[68], 68); // 68  right_buxian(x1,y1,x2,y2,
                sousuojieshuhang = 15;
            }

            if (r_start < 55) // 60
            {
                three_cross1 = 1;
                //                             uart_putchar(WIRELESS_UART, '6');
                //                                             uart_putchar(WIRELESS_UART, '4');
            }
            if (three_cross1 && r_start > 55) //||white_num_col_line>=130&&white_num_col_min_line>=110
            {
                three_cross = 0;
                three_cross1 = 0;
                three_cross_cnt++;
                //  sprintf(uart_buf,"%d          /r/n",three_cross_cnt);
                //        seekfree_wireless_send_buff(uart_buf, 10);
                //                               uart_putchar(WIRELESS_UART, '6');
                //                                              uart_putchar(WIRELESS_UART, '5');
                //                                                uart_putchar(WIRELESS_UART, ' ');
            }
        }
        else if (park_flag == 0)
        {
            if (sousuojieshuhang <= 15) //&&(white_num_col_min_line>20)
            {
                if (findleftdownguai) // 第一圈
                    left_buxian(white_num_col_min_line, 68 - white_num_col[white_num_col_min_line], left_turn_down[1], left_turn_down[0]);
                else
                    left_buxian(white_num_col_min_line, 68 - white_num_col[white_num_col_min_line], left_line[68], 68);
                for (int ql = 68 - white_num_col[white_num_col_min_line] + 2; ql >= 10; ql--)
                {
                    Right_Line_New[ql] = 180;
                    Left_Line_New[ql] = 130;
                }

                sousuojieshuhang = 15;
            }
            else
            {
                left_buxian(185, 15, left_line[68], 68);
                sousuojieshuhang = 15;
            }

            if (l_start < 55)
            {
                three_cross1 = 1;
                //                           uart_putchar(WIRELESS_UART, '6');
                //                                            uart_putchar(WIRELESS_UART, '2');
            }

            if (three_cross1 && l_start > 55) //&&white_num_col_min_line<70
            {
                three_cross_cnt++;
                //                               sprintf(uart_buf,"%d          /r/n",three_cross_cnt);
                //                               seekfree_wireless_send_buff(uart_buf, 10);
                three_cross = 0;
                three_cross1 = 0;
                //                               uart_putchar(WIRELESS_UART, '6');
                //                                               uart_putchar(WIRELESS_UART, '3');    uart_putchar(WIRELESS_UART, ' ');
            }
        }
    }

    // 在十字里
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
    unsigned int i;
    for (i = 68; i > count; i--)
    {
        //// 赛道宽度动态调整
        Half_width_handao[i] = Half_width_handao2[i] + huan_bankuan;
        Half_width[i] = Half_width2[i] + com_bankuan;
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
                    center[i] = Right_Line_New[i] - Half_width_yuanshi[i]; //||huihuan_num>15||sousuojieshuhang<=3
                else
                {
                    if (right_huan_num != 0 || left_huan_num != 0)
                        center[i] = Right_Line_New[i] - Half_width_handao[i];
                    else
                        center[i] = Right_Line_New[i] - Half_width[i];
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
    for (i = 60; i > count; i--)
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
    for (i = 68; i > sousuojieshuhang; i--)
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

    //   if (Foresight_Left + 42 < Foresight_Right&&!youhuandao_flag&&!zuohuandao_flag)     //位于直线或小S且没有障碍物
    //  {
    //          Point = (Foresight_Left + Foresight_Right) / 2; //取左右极值中点作为目标点
    //  }
    //                 else
    //                 {
    // if(park_flag==1&&j_flag==0) time_flag1=time_flag,j_flag=1;

    //
    //                    if((left_num+right_num)/2<340)
    //                    quanzhi_num=23;
    //                else  if((left_num+right_num)/2<355)
    //                       quanzhi_num=22;
    //                else  if((left_num+right_num)/2<370)
    //                       quanzhi_num=20;
    //                else  if((left_num+right_num)/2<385)
    //                       quanzhi_num=18;
    //                else  if((left_num+right_num)/2<430)
    //                       quanzhi_num=16;
    //                else  if((left_num+right_num)/2<480)
    //                       quanzhi_num=15;
    //
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
    // else
    //  j_flag=0;

    //      if(poer_flag)
    //      {
    //          for(int i=0;i<70;i++)
    //        weight_jubu[i]=Weight_park[69-i];
    //
    //      }
    //        else if(left_huan_num!=0||right_huan_num!=0)
    //        {
    //           for(int i=0;i<70;i++)
    //         weight_jubu[i]=Weight_huandao[i];
    //        }
    //         else  if(lefthuihuan_flag==1||youhuihuan_flag==1)
    //         {
    //         for(int i=0;i<70;i++)
    //         weight_jubu[i]=Weight_huihuan[i];
    //         }

    //        else  if(lefthuihuan_flag==1||youhuihuan_flag==1)
    //        {
    //        for(int i=0;i<70;i++)
    //        weight_jubu[i]=Weight_huihuan[i];
    //        }
    //      else
    //      {
    //          for(int i=0;i<70;i++)
    //            weight_jubu[i]=Weight[i];
    //      }

    //      if((left_num+right_num)/2<340)
    //                    quanzhi_num=22;
    //                else  if((left_num+right_num)/2<355)
    //                       quanzhi_num=21;
    //                else  if((left_num+right_num)/2<370)
    //                       quanzhi_num=19;
    //                else  if((left_num+right_num)/2<385)
    //                       quanzhi_num=17;
    //                else  if((left_num+right_num)/2<400)
    //                       quanzhi_num=15;
    //                else  if((left_num+right_num)/2<420)
    //                       quanzhi_num=14;
    //                else  if((left_num+right_num)/2<440)
    //                        quanzhi_num=13;

    if (park_flag == 1 && star_lineflag == 1)
    {
        for (i = 68; i >= 1; i--) // 使用加权平均
        {

            Sum += center[i] * weight_jubu[i];
            Weight_Count += weight_jubu[i];
        }
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
            Points = Sum / Weight_Count;
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

    Points = Point_last1 * 0.7 + Point_last2 * 0.2 + Point_last3 * 0.1;

    /***** 使用最远行数据和目标点作为前瞻 *****/
    if (sousuojieshuhang < 25)
    {
        Point_Mid = center[25];
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

int range_protect(int duty,int min,int max) // 限幅保护
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
    times2 = 0;////times2为指定行斑马线数
    for (unsigned int y = 21; y <= 29; y++) // 27  32//斑马线识别（如果识别迟了就调小点，识别早了就大点）
    {
        black_blocks = 0;
        cursor = 0; // 指向栈顶的游标
        for (unsigned int x = left_line[35] + 5; x <= right_line[35] - 5; x++)////+-5防止边缘干扰
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
        }////black_blocks记录总的斑马线块数，cursor记录当前斑马线块的长度
        if (black_blocks >= 3 && black_blocks <= 10)
            times2++;
    }
    if (times2 >= 3 && times2 <= 9) // 6
    {
        star_lineflag = 1;////起跑线标志位
    }
    else
    {
        star_lineflag = 0;
    }
}
