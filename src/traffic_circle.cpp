#include "traffic_circle.h"
#include "isr.h"
#include "my_control.h"
int youhuihuan_flag = 0, lefthuihuan_flag = 0;
int zuohuandao_flag = 0, zuohuandao_flag2 = 0, zuohuandao_flag3 = 0, buxianflag = 0;
int zuochu_flag = 0, zuochu_flag2 = 0;
int youhuandao_flag = 0, youhuandao_flag2 = 0, youhuandao_flag3 = 0, youhuandao_flag4 = 0, buxianflag2 = 0;
int youchu_flag = 0, youchu_flag2 = 0;
int zuohuandao_flag_queding = 0;
int m = 0, k = 0;
int bianzhai = 0, biankuan = 0;
int cnt2 = 0;
int line_you = 11, line_zuo = 11, n1 = 0, n2 = 0, n11 = 0, n22 = 0;
int jiesu_num1 = 0, jiesu_num2 = 0, jiesu_num11 = 0, jiesu_num22 = 0;
int huan_leijia = 0;

int num_y = 0, N1 = 0, N2 = 0;
int youhuandao_flag_queding = 0;
int tiao = 10; // 调变点
int right_turn_middle[2] = {0};
int left_turn_middle[2] = {0};
int leftx = 0, lefty = 0, rightx = 0, righty = 0, left_lianjiedian_x, left_lianjiedian_y, right_lianjiedian_x, right_lianjiedian_y;
int left_huan_num = 0, right_huan_num = 0;
int fine1 = 0, fine2 = 0;
int flag_find_huan_rightmiddle_point = 0;
int flag_find_huan_leftmiddle_point = 0;
int flag_find_huan_leftup_point = 0;
int flag_find_huan_rightup_point = 0;
extern int right_turn_up[2];
void find_rightup_point(int start_point, int end_point)////5  65
{
    int j;

    for (j = end_point; j >= start_point; j--)
    {
        /***找右上拐点***********/

        if (((j < (int)right_turn_down[0]) && right_line[j + 3] - right_line[j - 3] >= 30 && right_line[j + 4] - right_line[j - 4] >= 30 && right_line[j + 2] - right_line[j - 2] >= 30)
            //   &&right_line[j + 3]>170&&right_line[j +4]>170&&right_line[j +5]>170
            && (Right_Add[j - 2] == 0 && Right_Add[j - 3] == 0 && Right_Add[j - 4] == 0))
        {
            flag_find_huan_rightup_point = 1;

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
}
void find_rightup_point2(int start_point, int end_point)
{
    int j;

    for (j = end_point; j >= start_point; j--)
    {
        /***找右上拐点***********/

        if (((j < (int)right_turn_down[0]) && right_line[j + 3] - right_line[j - 3] >= 30 && right_line[j + 4] - right_line[j - 4] >= 30 && right_line[j + 5] - right_line[j - 5] >= 30)
            //   &&right_line[j + 3]>170&&right_line[j + 4]>170&&right_line[j + 5]>170
            && (Right_Add[j - 3] == 0 && Right_Add[j - 4] == 0 && Right_Add[j - 5] == 0))
        {
            right_turn_up[0] = j - 3;
            right_turn_up[1] = right_line[j - 3];
            flag_find_huan_rightup_point = 1;
            if (right_turn_up[0] >= right_turn_down[0])
            {
                ;
            }
            else
                break;
        }
    }
}
void find_rightup_point3(int start_point, int end_point)
{
    int j;

    for (j = start_point; j <= end_point; j++)
    {
        /***找右上拐点***********/

        if (((j < (int)right_turn_down[0]) && right_line[j + 3] - right_line[j - 3] >= 15 && right_line[j + 4] - right_line[j - 4] >= 15 && right_line[j + 5] - right_line[j - 5] >= 15) && right_line[j + 3] > 180 && right_line[j + 4] > 180 && right_line[j + 5] > 180 && (Right_Add[j - 3] == 0 && Right_Add[j - 4] == 0 && Right_Add[j - 5] == 0))
        {
            right_turn_up[0] = j - 3;
            right_turn_up[1] = right_line[j - 3];
            flag_find_huan_rightup_point = 1;
            if (right_turn_up[0] >= right_turn_down[0])
            {
                ;
            }
            else
                break;
        }
    }
}
void find_leftup_point2(int start_point, int end_point)
{
    int j;

    for (j = end_point; j >= start_point; j--)
    {
        /***找右上拐点***********/

        if (((j < (int)left_turn_down[0]) && left_line[j - 3] - left_line[j + 3] >= 30 && left_line[j - 4] - left_line[j + 4] >= 30 && left_line[j - 5] - left_line[j + 5] >= 30)
            //   &&left_line[j + 3]<15&&left_line[j + 4]<15&&left_line[j + 5]<15
            && (Left_Add[j - 3] == 0 && Left_Add[j - 4] == 0 && Left_Add[j - 5] == 0))
        {
            left_turn_up[0] = j - 3;
            left_turn_up[1] = left_line[j - 3];
            flag_find_huan_leftup_point = 1;
            if (left_turn_up[0] >= left_turn_down[0])
            {
                ;
            }
            else
                break;
        }
    }
}
void find_leftup_point3(int start_point, int end_point)
{
    int j;

    for (j = start_point; j <= end_point; j++)
    {
        /***找右上拐点***********/

        if (((j < (int)left_turn_down[0]) && left_line[j - 3] - left_line[j + 3] >= 15 && left_line[j - 4] - left_line[j + 4] >= 15 && left_line[j - 5] - left_line[j + 5] >= 15)
            //          &&left_line[j + 3]<30&&left_line[j + 4]<30&&left_line[j + 5]<30
            && (Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0))
        {
            left_turn_up[0] = j - 3;
            left_turn_up[1] = left_line[j - 3];
            flag_find_huan_leftup_point = 1;
            if (left_turn_up[0] >= left_turn_down[0])
            {
                ;
            }
            else
                break;
        }
    }
}
void find_rightmiddle_point(int start_point, int end_point)
{
    int i;
    for (i = start_point; i >= end_point; i--)
    {
        // 找you拐点
        if (i <= 67 && (right_line[i - 5] - right_line[i - 1]) >= 0 && abs(right_line[i - 5] - right_line[i - 1]) <= 10 && (right_line[i - 4] - right_line[i]) >= 0 && abs(right_line[i - 4] - right_line[i]) <= 10 && (right_line[i + 4] - right_line[i]) >= 0 && abs(right_line[i + 4] - right_line[i]) <= 10 && (right_line[i + 5] - right_line[i]) >= 0 && abs(right_line[i + 5] - right_line[i]) <= 10 && right_line[i + 5] >= right_line[i + 2] && right_line[i - 5] >= right_line[i - 2] && right_line[i] < 180 && right_line[i - 2] < 180 && right_line[i - 5] < 180 && right_line[i + 2] < 180 && right_line[i + 5] < 180)
        {
            right_turn_middle[0] = (int)(i);
            right_turn_middle[1] = right_line[i];
            flag_find_huan_rightmiddle_point = 1;
            break;
        }
    }
}
void find_leftup_point(int start_point, int end_point)
{
    int j;

    for (j = end_point; j >= start_point; j--)
    {
        /***找右上拐点***********/

        if (((j < (int)left_turn_down[0]) && left_line[j - 3] - left_line[j + 3] >= 30 && left_line[j - 4] - left_line[j + 4] >= 30 && left_line[j - 2] - left_line[j + 2] >= 30)
            //       &&left_line[j + 3]<30&&left_line[j + 4]<30&&left_line[j + 5]<30
            && (Left_Add[j - 4] == 0 && Left_Add[j - 2] == 0 && Left_Add[j - 3] == 0))
        {
            left_turn_up[0] = j - 3;
            left_turn_up[1] = left_line[j - 3];
            flag_find_huan_leftup_point = 1;
            if (left_turn_up[0] >= left_turn_down[0])
            {
                ;
            }
            else
                break;
        }
    }
}

void find_leftmiddle_point(int start_point, int end_point)
{
    int i;
    for (i = start_point; i >= end_point; i--)
    {
        // 找you拐点
        //        if (i <= 67 && (left_line[i - 1] - left_line[i - 3]) >=0&& (left_line[i - 1] - left_line[i - 3]) <=4
        //                    &&(left_line[i ] - left_line[i -2]) >= 0 &&(left_line[i ] - left_line[i -2]) <=4
        //                    && (left_line[i + 1] - left_line[i + 3]) >=0 && (left_line[i + 1] - left_line[i + 3]) <=4
        //
        //                    && left_line[i -1] >5&& left_line[i] >5&& left_line[i + 1] >5&& left_line[i + 2] >5&& left_line[i + 3] >5
        //            )

        if (i <= 67 && (left_line[i - 6] - left_line[i - 1]) <= 0 && abs(left_line[i - 6] - left_line[i - 1]) <= 8 && (left_line[i - 4] - left_line[i]) <= 0 && abs(left_line[i - 4] - left_line[i]) <= 8 && (left_line[i + 4] - left_line[i]) <= 0 && abs(left_line[i + 4] - left_line[i]) <= 8 && (left_line[i + 6] - left_line[i]) <= 0 && abs(left_line[i + 6] - left_line[i]) <= 8 && left_line[i + 6] <= left_line[i + 2] && left_line[i - 6] <= left_line[i - 2] && left_line[i] > 10 && left_line[i - 2] > 10 && left_line[i - 6] > 10 && left_line[i + 2] > 10 && left_line[i + 6] > 10)
        {
            left_turn_middle[0] = (int)(i - 1);
            left_turn_middle[1] = left_line[i - 1];
            flag_find_huan_leftmiddle_point = 1;
            break;
        }
    }
}
// void find_leftmiddle_point(int start_point, int end_point)
//{
//     int i;
//     for (i = start_point; i >= end_point; i--)
//     {
//         //找you拐点
//         if (i <= 67 && (left_line[i - 1] - left_line[i - 3]) >=0&& (left_line[i - 1] - left_line[i - 3]) <=4
//                     &&(left_line[i ] - left_line[i -2]) >= 0 &&(left_line[i ] - left_line[i -2]) <=4
//                     && (left_line[i + 1] - left_line[i + 3]) >=0 && (left_line[i + 1] - left_line[i + 3]) <=4
//
//                     && left_line[i -1] >5&& left_line[i] >5&& left_line[i + 1] >5&& left_line[i + 2] >5&& left_line[i + 3] >5
//             )
//         {
//             left_turn_middle[0] = (int)(i - 1);
//             left_turn_middle[1] = left_line[i - 1];
//             flag_find_huan_leftmiddle_point = 1;
//             break;
//         }
//     }
//
// }
void left_buxian(int x1, int y1, int x2, int y2) // 左补线（环岛）
{
    int i, max, a1, a2, hx;
    a1 = y1;
    a2 = y2;
    if (a1 > a2)
    {
        max = a1;
        a1 = a2;
        a2 = max;
    }
    for (i = a1; i < a2; i++)
    {

        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= 184)
            hx = 184;
        if (hx <= 2)
            hx = 2;
        Left_Line_New[i] = hx;
    }
}
void left_buxian2(int x1, int y1, int x2, int y2) // 左补线（环岛）
{
    int i, max, a1, a2, hx;
    a1 = y1;
    a2 = y2;
    if (a1 > a2)
    {
        max = a1;
        a1 = a2;
        a2 = max;
    }
    for (i = a1; i < a2; i++)
    {

        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= 184)
            hx = 184;
        if (hx <= 2)
            hx = 2;
        Right_Line_New[i] = hx;
    }
}
void right_buxian2(int x1, int y1, int x2, int y2) // 右补线（环岛）
{
    int i, max, a1, a2, hx;
    a1 = y1;
    a2 = y2;
    if (a1 > a2)
    {
        max = a1;
        a1 = a2;
        a2 = max;
    }
    for (i = a1; i < a2; i++)
    {

        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= 184)
            hx = 184;
        if (hx <= 2)
            hx = 2;
        Left_Line_New[i] = hx;
    }
}

void right_buxian(int x1, int y1, int x2, int y2) // 右补线（环岛）
{
    int i, max, a1, a2, hx;
    a1 = y1;
    a2 = y2;
    if (a1 > a2)
    {
        max = a1;
        a1 = a2;
        a2 = max;
    }
    for (i = a1; i < a2; i++)
    {

        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        if (hx >= 184)
            hx = 184;
        if (hx <= 2)
            hx = 2;
        Right_Line_New[i] = hx;
    }
}

void youhuandao_deal() // 环岛处理
{
    int y = 0;
    find_rightdown_point(60, 10, 2);
    find_rightmiddle_point(60, 8);
    find_rightup_point(5, 65);
    if (right_huan_num == 1)
    {

        // if(flag_find_huan_rightmiddle_point)
        //{
        //   right_buxian(right_turn_middle[1],right_turn_middle[0],160,right_turn_down[0]);
        //   flag_find_huan_rightmiddle_point=0;
        // }
        right_buxian(left_line[15] + 35, 15, 170, 68);
        //  sousuojieshuhang=15;
    }
    else if (right_huan_num == 2)
    {
        // if(flag_find_huan_rightmiddle_point)
        // {
        //  right_buxian(right_turn_middle[1],right_turn_middle[0],160,68);
        //   flag_find_huan_rightmiddle_point=0;
        // }
        right_buxian(left_line[15] + 35, 15, 170, 68);
        // sousuojieshuhang=15;
    }
    else if (right_huan_num == 3)
    {

        //      if(flag_find_huan_rightmiddle_point)
        //         {
        //          flag_find_huan_rightmiddle_point=0;
        //          if(right_turn_up[0]>=13)//21
        //             {
        //              right_buxian2(right_turn_up[1]+14,right_turn_up[0],left_line[right_turn_middle[0]],right_turn_middle[0]);
        //             for(int ql=right_turn_up[0];ql>=10;ql--)
        //                   {
        //                      Right_Line_New[ql]=180;
        //                      Left_Line_New[ql]=140;
        //                   }
        //             }
        //         }
        //      else
        //      {
        if (right_turn_up[0] >= 12) // 21
        {
            right_buxian2(right_turn_up[1] + 30, right_turn_up[0], left_line[68], 68);
            for (int ql = right_turn_up[0]; ql >= 10; ql--)
            {
                Right_Line_New[ql] = 183;
                Left_Line_New[ql] = 140;
            }
            sousuojieshuhang = 15;
        }
        //   }
    }
    else if (right_huan_num == 4)
    {
        for (int ql = 68; ql >= 17; ql--)
        {
            Right_Line_New[ql] = 184;
            Left_Line_New[ql] = 67;
        }
        for (y = 68; y > 5; y--) // x是减59—56  num是加0—4
        {
            for (int x = 160; x <= 184; x++) // 中间向右找跳变
            {
                if (image_use[y][x - 1] == 255 && image_use[y][x] == 255 && image_use[y][x + 1] == 0 && image_use[y][x + 2] == 0) // 两个连续黑点触发
                {

                    Right_Line_New[y] = x;
                    break;
                }
                else if (x == 184) // 到最右边了都没扫到黑点a
                {

                    Right_Line_New[y] = x;

                    break;
                }
            }

            for (int x = 160; x >= 1; x--) // 中间向左找跳变
            {
                if (image_use[y][x - 1] == 0 && image_use[y][x] == 0 && image_use[y][x + 1] == 255 && image_use[y][x + 2] == 255) // 两个连续黑点触发
                {
                    Left_Line_New[y] = x + 1;
                    break;
                }
                else if (x + 1 == 67) // 到最左边了都没扫到黑点
                {
                    Left_Line_New[y] = x + 1;
                    break;
                }
            }
            if (abs(Right_Line_New[y] - Left_Line_New[y]) < 3)
                break;
            if (image_use[y][(Right_Line_New[y] + Left_Line_New[y]) / 2] == 0)
                break;
            if (y <= 1)
                break;
        }
        sousuojieshuhang = y + 1;
        left_buxian(right_turn_up[1], right_turn_up[0], 30, 68);
    }
    else if (right_huan_num == 5)
    {
        for (int ql = 68; ql >= 17; ql--)
        {
            Right_Line_New[ql] = 184;
            Left_Line_New[ql] = 67;
        }
        for (y = 68; y > 5; y--) // x是减59—56  num是加0—4
        {
            for (int x = 160; x <= 184; x++) // 中间向右找跳变
            {
                if (image_use[y][x - 1] == 255 && image_use[y][x] == 255 && image_use[y][x + 1] == 0 && image_use[y][x + 2] == 0) // 两个连续黑点触发
                {

                    Right_Line_New[y] = x;
                    break;
                }
                else if (x == 184) // 到最右边了都没扫到黑点a
                {

                    Right_Line_New[y] = x;

                    break;
                }
            }

            for (int x = 160; x >= 1; x--) // 中间向左找跳变
            {
                if (image_use[y][x - 1] == 0 && image_use[y][x] == 0 && image_use[y][x + 1] == 255 && image_use[y][x + 2] == 255) // 两个连续黑点触发
                {
                    Left_Line_New[y] = x + 1;
                    break;
                }
                else if (x + 1 == 67) // 到最左边了都没扫到黑点
                {
                    Left_Line_New[y] = x + 1;
                    break;
                }
            }
            if (abs(Right_Line_New[y] - Left_Line_New[y]) < 3)
                break;
            if (image_use[y][(Right_Line_New[y] + Left_Line_New[y]) / 2] == 0)
                break;
            if (y <= 1)
                break;
        }
        sousuojieshuhang = y + 1;
        left_buxian(right_turn_up[1] + 18, right_turn_up[0], 30, 68);
    }
    else if (right_huan_num == 7)
    {
        if (!(zuodiuxianshu < 15 && Left_Add_num < 3))
        {
            find_leftdown_point(65, 25, 1); // 1是十字
            right_buxian2(162, sousuojieshuhang, left_turn_down[1] + 20, left_turn_down[0]);
        }
    }

    else if (right_huan_num == 8)
    {

        right_buxian(left_line[15] + 55, 15, 170, 68);
    }
}

void zuohuandao_deal() // 环岛处理
{
    int y = 0;
    find_leftdown_point(65, 1, 2);
    find_leftmiddle_point(60, 8);
    find_leftup_point(5, 65);
    if (left_huan_num == 1)
    {

        // if(flag_find_huan_leftmiddle_point)
        //{
        //   left_buxian(left_turn_middle[1],left_turn_middle[0],30,68);
        //   flag_find_huan_leftmiddle_point=0;
        // }
        // else
        {
            left_buxian(right_line[15] - 40, 15, 35, 68);
            //  flag_find_huan_leftmiddle_point=0;
        }
    }
    else if (left_huan_num == 2)
    {
        // if(flag_find_huan_leftmiddle_point)
        // {3
        //     left_buxian(left_turn_middle[1],left_turn_middle[0],30,68);
        //      flag_find_huan_leftmiddle_point=0;
        //  }
        // else
        {
            left_buxian(right_line[15] - 40, 15, 35, 68);
            //  flag_find_huan_leftmiddle_point=0;
        }
    }
    else if (left_huan_num == 3)
    {

        if (left_turn_up[0] >= 13) // 21
        {

            left_buxian2(left_turn_up[1] - 25, left_turn_up[0], right_line[68], 68);

            for (int ql = left_turn_up[0]; ql >= 10; ql--)
            {
                Left_Line_New[ql] = 20;
                Right_Line_New[ql] = 55;
            }
            sousuojieshuhang = 15;
        }
    }
    else if (left_huan_num == 4)
    {

        for (int ql = 68; ql >= 17; ql--)
        {
            Left_Line_New[ql] = 2;
            center[ql] = 10;
        }

        for (y = 68; y > 10; y--) // x是减59—56  num是加0—4
        {
            for (int x = 40; x <= 184; x++) // 中间向右找跳变
            {
                if (image_use[y][x - 1] == 255 && image_use[y][x] == 255 && image_use[y][x + 1] == 0 && image_use[y][x + 2] == 0) // 两个连续黑点触发
                {

                    Right_Line_New[y] = x;

                    break;
                }
                else if (x == 184) // 到最右边了都没扫到黑点a
                {

                    Right_Line_New[y] = x;

                    break;
                }
            }

            for (int x = 40; x >= 1; x--) // 中间向左找跳变
            {
                if (image_use[y][x - 1] == 0 && image_use[y][x] == 0 && image_use[y][x + 1] == 255 && image_use[y][x + 2] == 255) // 两个连续黑点触发
                {
                    Left_Line_New[y] = x + 1;
                    break;
                }
                else if (x + 1 == 2) // 到最左边了都没扫到黑点
                {
                    Left_Line_New[y] = x + 1;
                    break;
                }
            }
            if (abs(Right_Line_New[y] - Left_Line_New[y]) < 3)
                break;
            if (image_use[y][(Right_Line_New[y] + Left_Line_New[y]) / 2] == 0)
                break;
            if (y <= 1)
                break;
        }
        sousuojieshuhang = y + 1;
        right_buxian(left_turn_up[1], left_turn_up[0], 172, 68);
    }
    else if (left_huan_num == 5)
    {

        for (int ql = 68; ql >= 17; ql--)
        {
            Left_Line_New[ql] = 18;
            center[ql] = 10;
            Right_Line_New[ql] = 135;
        }

        for (y = 68; y > 10; y--) // x是减59—56  num是加0—4
        {
            for (int x = 40; x <= 184; x++) // 中间向右找跳变
            {
                if (image_use[y][x - 1] == 255 && image_use[y][x] == 255 && image_use[y][x + 1] == 0 && image_use[y][x + 2] == 0) // 两个连续黑点触发
                {

                    Right_Line_New[y] = x;

                    break;
                }
                else if (x == 135) // 到最右边了都没扫到黑点a
                {

                    Right_Line_New[y] = x;

                    break;
                }
            }

            for (int x = 40; x >= 1; x--) // 中间向左找跳变
            {
                if (image_use[y][x - 1] == 0 && image_use[y][x] == 0 && image_use[y][x + 1] == 255 && image_use[y][x + 2] == 255) // 两个连续黑点触发
                {
                    Left_Line_New[y] = x + 1;
                    break;
                }
                else if (x + 1 == 2) // 到最左边了都没扫到黑点
                {
                    Left_Line_New[y] = x + 1;
                    break;
                }
            }
            if (abs(Right_Line_New[y] - Left_Line_New[y]) < 3)
                break;
            if (image_use[y][(Right_Line_New[y] + Left_Line_New[y]) / 2] == 0)
                break;
            if (y <= 1)
                break;
        }
        sousuojieshuhang = y + 1;

        right_buxian(left_turn_up[1], left_turn_up[0], 172, 68);
    }

    else if (left_huan_num == 7)
    {
        if (!(youdiuxianshu < 15 && Right_Add_num < 3))
        {
            find_rightdown_point(65, 25, 1);                                            // 1是十字
            left_buxian2(55, sousuojieshuhang, right_turn_down[1], right_turn_down[0]); // 40
        }
    }
    else if (left_huan_num == 8)
    {

        left_buxian(right_line[20] - 70, 20, 10, 68);
    }
}

void youhuandao() // 右环岛
{
    guaidian = 0;
    if (!youhuihuan_flag && !left_huan_num && !three_cross && !poer_flag && !star_lineflag && !por_cnt) //
    {
        find_rightup_point(10, 65);
        find_rightdown_point(65, 22, 2);
        right_turn_down[0] = 69;
        m = Right_Add_num - Left_Add_num;
        if (right_huan_num == 0 && !right_huan_num && (m > 11) && sousuojieshuhang <= 9 && l_start >= 65 && r_start >= 65 && (Right_Add_num >= 17) && (Left_Add_num <= 3) && (zuodiuxianshu < 3) && (youdiuxianshu > 17) && k_left < -0.6 && k_left > -1.3)
        {
            youhuandao_flag = 1;
            right_huan_num = 1;

            time3 = 0;
        }
        else if (right_huan_num == 1 && l_start >= 65 && r_start <= 40) //&&(ad_guiyi[2]>100||ad_guiyi[0]>100)
        {
            right_huan_num = 2;
            huan2_flag = 1;
        }
        else if (right_huan_num == 2)
        {
            find_rightmiddle_point(50, 20);
            guaidian = right_turn_middle[0];
            if (huan2_flag == 0)
                right_huan_num = 0;
            if (right_huan_num == 2 && r_start >= 67 && guaidian >= 22) // l_start>= 50&&r_start<=35
            {
                cnt2++; // 连续3次判定
            }
            else
                cnt2 = 0;

            if (cnt2 >= 2 && gou_Guo2 == 0)
            {

                right_huan_num = 3;

                cnt2 = 0;
            }
        }
        else if (right_huan_num == 3 && right_turn_up[0] > 35) //>40)//right_turn_up[0]////right_huan_num==3&&r_start<=40
        {
            guaidian = 0;
            right_huan_num = 5; // 更换搜线方式
        }
        else if (right_huan_num == 5 && white_num_col_line > 130)
        {
            right_huan_num = 6;
        }

        else if (right_huan_num == 6 && Left_Add_num > 8 && left_turn_down[0] > 25 && left_turn_down[0] != 69) //&&left_turn_down[0]<50
        {
            right_huan_num = 7;
        }
        else if (right_huan_num == 7 && sousuojieshuhang <= 15 && zuodiuxianshu < 2 && Left_Add_num < 3 && (huandao_7 >= 10 || (k_left < -0.6 && k_left > -1.3))) //&&zuodiuxianshu<5
        {
            right_huan_num = 8; //&&white_num_col[93]>50
        }
        else if (right_huan_num == 8 && r_start < 60)
        {
            right_huan_num = 9;
        }

        else if (right_huan_num == 9 && l_start > 60 && r_start > 60)
        {

            youhuandao_flag = 0;
            youhuandao_flag_queding = 0;
            youhuandao_flag2 = 0;
            right_huan_num = 0;
            time2 = 0;
            youhuandao_flag3 = 0;
        }
    }
    youhuandao_deal();
}

void you_huihuan()
{

    if (right_huan_num == 2 && youhuihuan_flag == 0 && huihuan_num < 16 && r_start < 60) //&&sousuojieshuhang>5//
    {

        youhuihuan_flag = 1;
        flag_find_huan_rightup_point = 0;
        right_huan_num = 0;
    }
    else if (youhuihuan_flag == 1 && r_start > 65)
        youhuihuan_flag = 2;
    else if (youhuihuan_flag == 2 && Left_Add_num > 11 && zuodiuxianshu > (17 + huihuan_right) && youdiuxianshu > (17 + huihuan_right)) //&&Left_Add_num>29
    {
        {
            youhuihuan_flag = 3; // 26
        }
    }

    else if (youhuihuan_flag == 3 && r_start > 66 && sousuojieshuhang <= 15 && huandao_7 >= 10) //&&white_num_col[93]>60
    {
        youhuihuan_flag = 0;
    }
    if (youhuihuan_flag == 3)
    {
        for (int ql = 68; ql >= 10; ql--)
        {
            Left_Line_New[ql] = 6;
            Right_Line_New[ql] = 70;
        }
    }
}

void zuo_huihuan()
{

    if (left_huan_num == 2 && lefthuihuan_flag == 0 && huihuan_num < 16 && l_start < 60) //
    {

        lefthuihuan_flag = 1;
        flag_find_huan_leftup_point = 0;
        left_huan_num = 0;
    }
    else if (lefthuihuan_flag == 1 && l_start > 65)
        lefthuihuan_flag = 2;
    else if (lefthuihuan_flag == 2 && Right_Add_num > 10 && left_huan_num == 0 && zuodiuxianshu > (10 + huihuan_left) && youdiuxianshu > (10 + huihuan_left))
    {
        lefthuihuan_flag = 3; // 快15   慢20
    }

    else if (lefthuihuan_flag == 3 && sousuojieshuhang <= 15 && l_start > 66 && huandao_7 >= 10) //&&huandao_7>=25
    {
        lefthuihuan_flag = 0;
    }
    if (lefthuihuan_flag == 3)
    {
        for (int ql = 68; ql >= 10; ql--)
        {
            Left_Line_New[ql] = 130;
            Right_Line_New[ql] = 180;
        }
    }
}
void zuohuandao() // zuo环岛
{
    guaidian = 0;
    if (!lefthuihuan_flag && !right_huan_num && !three_cross && !poer_flag && !star_lineflag && !por_cnt) //
    {

        left_turn_down[0] = 69;
        m = Left_Add_num - Right_Add_num; //
        find_leftup_point(10, 65);
        find_rightdown_point(65, 22, 2);
        if (!left_huan_num && left_huan_num == 0 && (m > 11) && sousuojieshuhang <= 9 && l_start >= 60 && r_start >= 60 && (Left_Add_num >= 15) && (Right_Add_num <= 3) && (zuodiuxianshu > 15) && (youdiuxianshu < 3) && k_right > 0.6 && k_right < 1.3)
        { //&&k_right>0.7&&k_right<2.0&&k_left<-0.7&&k_left>-2.0
            zuohuandao_flag = 1;
            left_huan_num = 1;
            time1 = 0;
        }
        else if (left_huan_num == 1 && r_start >= 65 && l_start <= 50 && l_start >= 25) //&&(ad_guiyi[2]>100||ad_guiyi[0]>100)
        {
            time1 = 0;
            huan2_flag = 1;

            left_huan_num = 2;
        }
        else if (left_huan_num == 2)
        {
            find_leftmiddle_point(50, 20);
            guaidian = left_turn_middle[0];
            if (huan2_flag == 0)
                left_huan_num = 0;
            if (left_huan_num == 2 && l_start >= 67 && guaidian > 25) // 35//
            {
                cnt2++; // 连续3次判定
                if (cnt2 >= 2 && gou_Guo2 == 0)
                {
                    cnt2 = 0;
                    // left_huan_num=21;
                    left_huan_num = 3;
                }
            }
            else
                cnt2 = 0;
        }
        else if (left_huan_num == 3 && (left_turn_up[0] > 40)) //>30)//right_turn_up[0]////right_huan_num==3&&r_start<=40
        {
            left_huan_num = 5; // 更换搜线方式
        }
        else if (left_huan_num == 5 && (white_num_col_line < 65))
        {
            left_huan_num = 6;
        }
        else if (left_huan_num == 6 && Right_Add_num > 8 && right_turn_down[0] > 28 && right_turn_down[0] != 69)
        {
            left_huan_num = 7;
        }
        else if (left_huan_num == 7 && sousuojieshuhang <= 15 && youdiuxianshu < 2 && Right_Add_num < 3 && (huandao_7 >= 10 || (k_right > 0.6 && k_right < 1.3))) //&&white_num_col[93]>50
        {
            left_huan_num = 8; //&&Right_Add_num<3  28
        }
        else if (left_huan_num == 8 && l_start < 60)
            left_huan_num = 9;

        else if (left_huan_num == 9 && l_start > 67 && r_start > 67)
        {
            cnt2++;
            cnt2 = 0;
            zuohuandao_flag = 0;
            zuohuandao_flag_queding = 0;
            zuohuandao_flag2 = 0;
            left_huan_num = 0;
            time7 = 0;
            zuohuandao_flag3 = 0;
        }
    }
    zuohuandao_deal();
}

// 【Function7】找右中拐点函数
// 输入： start点 end点
// void find_rightmiddle_point(int start_point, int end_point)
//{
//     int i;
//     for (i = start_point; i <= end_point; i--)
//     {
//         //找you拐点
//         if (i >= 8 && (right_line[i + 5] - right_line[i + 1]) > 0 && (right_line[i + 4] - right_line[i + 1]) > 0 && (right_line[i + 3] - right_line[i + 1]) > 0
//             && (right_line[i + 5] - right_line[i + 4]) >= 0 && (right_line[i + 4] - right_line[i + 3]) >= 0 && (right_line[i - 3] - right_line[i - 2]) >= 0
//             && (right_line[i - 4] - right_line[i + 1]) >= 0 && (right_line[i - 3] - right_line[i + 1]) >= 0 && (right_line[i - 2] - right_line[i + 1]) >= 0
//             && (right_line[i - 4] - right_line[i - 3]) >= 0 && (right_line[i - 3] - right_line[i - 2]) >= 0
//             )
//         {
//             right_turn_middle[0] = (int)(i - 1);
//             right_turn_middle[1] = right_line[i - 1];
//             flag_find_huan_rightmiddle_point = 1;
//             break;
//         }
//     }
//
// }
