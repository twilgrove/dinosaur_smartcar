#ifndef _traffic_circle_h
#define _traffic_circle_h

#include <cmath>
#include "headfile.h"


void you_huihuan();
void youhuandao();
void zuo_huihuan();
void zuohuandao();
void left_buxian(int x1,int y1,int x2,int y2);
void right_buxian(int x1,int y1,int x2,int y2);
void left_buxian2(int x1,int y1,int x2,int y2);
void right_buxian2(int x1,int y1,int x2,int y2);

void find_rightmiddle_point(int start_point, int end_point);
void find_rightup_point(int start_point, int end_point);
void find_leftmiddle_point(int start_point, int end_point);
void find_leftup_point(int start_point, int end_point);
extern int huan_leijia;
extern int youhuandao_flag_queding;
extern int zuohuandao_flag_queding;
extern int zuohuandao_flag ,zuohuandao_flag2,zuohuandao_flag3;
extern int zuochu_flag ,zuochu_flag2;
extern int youhuandao_flag ,youhuandao_flag2,youhuandao_flag3;
extern int youchu_flag ,zuochu_flag;
extern int line_you,line_zuo,n1,n2,n11,n22;
extern int righty,lefty;
void find_rightup_point2(int start_point, int end_point);
void find_leftup_point2(int start_point, int end_point);
void find_rightup_point3(int start_point, int end_point);
extern  int bianzhai;
extern  int biankuan;
extern  int guaidian;
extern int youhuihuan_flag;
extern int  lefthuihuan_flag;
extern int right_turn_middle[2];
extern int left_turn_middle[2];
extern int flag_find_huan_rightmiddle_point;
extern int flag_find_huan_leftmiddle_point;
extern int m, k;
extern int cnt2;
void zuo_huihuan();
#endif
