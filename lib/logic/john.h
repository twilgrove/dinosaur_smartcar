#ifndef _JOHN_H_
#define _JOHN_H_

#include "headfile.h"
#include "image_deal.h"
#include <chrono>
#include <iostream>

enum TrackKind
{
    LINE,
    WAN
};

struct JOHNTURN
{
    TrackKind Track_kind; // 赛道类型
    double servo_turn;    // 偏转误差
    float para;           // 最长白列参数
    double K_Turn_;       // 阿克曼后轮系数
};

extern JOHNTURN Control_john;

inline float DEGTORAD(float angle)
{
    return angle * 0.017453292519943295f; // π/180
}

inline float FastTan(float rad)
{
    float x = rad;
    float x2 = x * x;
    return x + (x2 * x) / 3.0f + (2.0f * x2 * x2 * x) / 15.0f;
}

double calc_rss_quadratic(const std::vector<double> &y);
std::pair<double, double> calc_rss_linear(const std::vector<double> &y);
std::vector<double> first_order_filter(const std::vector<double> &input_i, double alpha);
void transpose_matrix(const unsigned char *src, unsigned char *dst, int rows, int cols);

void Tread_Init();
TrackKind Choose_Kind(JOHNTURN *Control);
double Get_Turn(uint32_t sp_duty);
void Get_Error(JOHNTURN *Control);
void Get_Kp(JOHNTURN *Control);
void Get_Speed(int chujie, JOHNTURN *Control);
void Get_Sp_Duty(JOHNTURN *Control);

#endif
