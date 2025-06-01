/*
 * image_deal.h
 *
 *  Created on: 2022年5月21日
 *      Author: admin
 */

#ifndef CODE_IMAGE_DEAL_H_
#define CODE_IMAGE_DEAL_H_

#include <cmath>
#include "headfile.h"

void yuzhiget(void);
extern int times2;
extern int black_blocks;
extern int qvlv_quanju;
extern int cursor; // 指向栈顶的游标
extern int right_turn_down[2];
extern int left_turn_down[2];
extern int white_num_col_max;
extern int left_turn_up[2];
extern int findrightdownguai;
extern int findleftdownguai;
extern int white_num_col_line, white_num_col_min_line;
extern float k_left;
extern float k_right;
extern int r_start;
extern int l_start;
extern int times;
extern float curvity_left, curvity_right;
extern int xielv_1eft2;
extern int xielv_right2;
extern int Left_Line_New[70], Right_Line_New[70];
extern int three_cross_cnt;
void twovaltrans(int t1, int t2, int t3);
extern float k_center;
extern int star_lineflag, star_lineflag2;
long long int range_protect2(long long int duty, long long int min, long long int max); // 限幅保护

void Center_line_deal(); // 中线处理
void line_deal();
int my_adapt_threshold(int *image, long int col, long int row);
float Point_Weight(void);
void Left_Line_Repair(int y, int x);
void Right_Line_Repair(int y, int x);          // 右边界补线修正
int range_protect(int duty, int min, int max); // thy
void Mid_Line_Repair(int count);
void star_line_judg2();
extern int guaidian;
extern int white_num_col[188];
extern int zuodiuxianshu;  // 左丢线数
extern int zongdiuxianshu; // 总丢线数
extern int Point_Mid, Get_Point, Foresight,Points;
extern int left_huan_num, right_huan_num; // 环岛变量
extern int park_flag;                     // 停车标志
extern int left_line[70], right_line[70]; // 左边界右边界
extern int sousuojieshuhang;              // 搜索结束行
extern int youdiuxianshu;                 // 右丢线数
extern int zuodiuxianshu;                 // 左丢线数
extern int Left_Add[70], Right_Add[70], Left_Add_num, Right_Add_num, Left_Add_num2, Right_Add_num2;
extern int n;
extern int n111;
extern int right_turn_up[2];
extern int three_cross;
extern int flag_shizi;
extern int Left_Add2[70], Right_Add2[70];
extern int three_cross1;
extern int kk, bai_flag, hei_flag, baihei_flag, heibai_flag, width_heibai;
extern int Right_Add[70];
extern int Left_Add[70];
extern int twolines_trend;
extern int left_line[70], right_line[70]; // 左边界右边界
extern int center[70], Width[70];
extern int Right_Add_num, Left_Add_num;
extern int white_num_col[188], white_num_col_line;

extern int threshold1, threshold1_old;
extern int Thresholds[3];
extern unsigned int Half_width_yuanshi[70];
extern int j_flag;
extern int check_line;
extern int cnt3;
extern unsigned int xk, xj;
unsigned int my_sqrt(int num);
void Cal_losttimes(int times);
void regression(int type, int startline, int endline);
void find_leftdown_point(int start_point, int end_point, int RoadName);
void find_rightdown_point(int start_point, int end_point, int RoadName);
char oppositeSigns(int x, int y);

void check_starting_line();
extern int xielv;
extern int huihuan_left;
extern int huihuan_right;
extern int whitenum;
extern int huihuan_num, zhidao_num, huandao_7;
extern int findrightupguai;
extern int findleftupguai;
extern int huan2_flag;
extern float parameterB, parameterA;
extern float trend_of_left, trend_of_right;
extern int th_y;
extern unsigned int Half_width_handao[70];
extern unsigned int const Half_width_handao2[70];
extern unsigned int const Half_width2[70];
extern unsigned int Half_width[70];
extern int qvlv_quanju, qulv_jinduan, qulv_yuandaun;

#endif /* CODE_IMAGE_DEAL_H_ */
