/*
 * PID.c
 *
 *  Created on: 2022年5月21日
 *      Author: admin
 */

#include "image_deal.h"
#include "thread.h"
#include "isr.h"
#include "key_board.h"
#include "PID.h"
#include "my_control.h"
#include "traffic_circle.h"
#include "headfile.h"
float Kp1; // 动态P
int Increase_last1 = 0, Increase_last2 = 0, Increase1 = 0, Increase2 = 0;
float det_Kp;
float det_Ki;
float det_Kd;                                                                                                                                                                                             // 12, 1, 28,180  MAX16  顺
PID S_D5_PID, MOTOR_PID, MOTOR2_PID;                                                                                                                                                                      // 定义舵机和电机的PID参数结构体//   11, 1, 50,135 回环
long long int S_D5[10][4] = {{14, 0, 55, 130}, {14, 0, 55, 130}, {14, 0, 55, 130}, {14, 0, 55, 130}, {14, 0, 55, 130}, {14, 0, 55, 130}, {14, 0, 55, 130}, {14, 0, 55, 130}, {14, 0, 55, 130}, {14, 0, 55, 130}}; // 舵机PID

//{ 20, 1, 0, 19}, {24, 2, 15, 28}, {22, 3, 24, 14}, {23, 4, 29, 15}, {25, 5, 21, 14}, {26, 6, 18, 14},},{28, 7, 32, 14} {27,8,35,12};
/* PID 的 I 项没有使用，只用 PD6此处 I 仅作为速度模式的标号，对PID无影响 */
// 4  200       20000 5  1   30    172
// 位置式PID参数初始化

float MOTOR[3] = {23, 0, 3}; // 电机PID

// float   MOTOR[3] = {5 , 0.2, 0}; //电机PID  18, 0.4
// float   MOTOR2[3] = {5 , 0.2, 0}; //电机PID18, 0.4
unsigned int BangBang_Flag = 0;
void PlacePID_Init(PID *sptr)
{
    sptr->SumError = 0;
    sptr->LastError = 0; // Error[-1];0
    sptr->PrevError = 0; // Error[-2]
}
// 增量式PID参数初始化
void IncPID_Init(PID *sptr)
{
    sptr->SumError = 0;
    sptr->LastError = 0; // Error[-1]
    sptr->PrevError = 0; // Error[-2]
    sptr->LastSpeed = 0;
}

// 位置式PID舵机控制
long long int PlacePID_Control(PID *sprt, long long int NowPiont, long long int SetPoint)
{
    // 定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
    register long long int iError, // 当前误差
        Actual, Actual2;   // 最后得出的实际输出值

    iError = NowPiont - SetPoint; // 计算当前误差

    Kp1 = 1.0 * (iError * iError) / S_D5[Set][KT] + S_D5[Set][KP]; // P值与差值成二次函数关系//
    if (Kp1 >= 16)
        Kp1 = 16;

    if (left_huan_num != 0 || right_huan_num != 0)
    {

        S_D5[Set][KD] = Kp1 * (KDD + 1) / 10.0; // 2.1  2.7
    }
    else
        S_D5[Set][KD] = Kp1 * KDD / 10.0; // 2.1  2.7
    // S_D5[Set][KD]=40;
    Actual = Kp1 * iError + S_D5[Set][KD] * (iError - sprt->LastError); // 只用PD
    Actual = Actual / 9;
    sprt->LastError = iError; // 更新上次误差

    Actual2 = Actual + stree_center;
    if (Actual2 >= stree_max)
        Actual2 = stree_max;
    if (Actual2 <= stree_min)
        Actual2 = stree_min;
    return Actual2;
}

// long long int PID_Cascade(PID *sptr, long long int ActualSpeed, long long int SetSpeed)
//{
//     //当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
//     register long long int iError,      //当前误差
//         Increase;   //最后得出的实际增量
//
//
//     iError = SetSpeed - ActualSpeed;//计算当前误差
//
//     Increase = - MOTOR[KP] * (ActualSpeed - sptr->LastSpeed)    //加速度
//                + MOTOR[KI] * iError
//                 + MOTOR[KD] * (iError - 2 * sptr->LastError + sptr->PrevError);\
//    sptr->PrevError = sptr->LastError;  //更新前次误差
//     sptr->LastError = iError;           //更新上次误差
//     sptr->LastSpeed = ActualSpeed;      //更新上次速度
//     if (iError < -70)
//     {
//         BangBang_Flag = 1;
//         Increase -= 100;
//     }
//     else if (iError < -50)
//     {
//         BangBang_Flag = 1;
//         Increase -= 70;
//     }
//     else if (iError < -30)
//     {
//         BangBang_Flag = 1;
//         Increase -= 30;
//     }
//
//     if ((iError > 50) && (BangBang_Flag == 1))
//     {
//         BangBang_Flag = 0;
//         Increase +=  150;
//     }
//     else if (iError > 50)
//     {
//         Increase += 100;
//     }
////   if(park_flag==0)
////             {
////
////                         if (iError < -300)
////                        {
////                            Increase -= 1000;
////                        }
////                                      if (iError < -200)
////                        {
////                            Increase -=800 ;
////                        }
////                                    else  if (iError < -100)
////                        {
////                            Increase -= 500;
////                        }
////                                    else if(iError < -70)
////                        {
////                            Increase -= 200;
////                        }
////
////
////                                    else  if (iError >200)
////                        {
////                            Increase += 400;
////                        }
////                                    else if (iError >100)
////                        {
////                            Increase += 200;
////                        }
////               }
////               else if(park_flag==1)
////               {
////                   if (iError < -200)
////                        {
////                            Increase -=10000 ;
////                        }
////                                      if (iError < -100)
////                        {
////                            Increase -=1000 ;
////                        }
////
////               }
//
//
//
///*        if (iError < -100)
//    {
//        Increase -= 200;
//    }
//    else if (iError < -80)
//    {
//        Increase -= 150;
//    }
//    else if (iError < -50)
//    {
//        Increase -= 125;
//    }
//        else if (iError < -30)
//        {
//                Increase -= 100;
//        }*/
//
//    return Increase;
//
//}

long long int PID_Cascade(PID *sptr, long long int ActualSpeed, long long int SetSpeed)
{
    // 当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
    register long long int iError, iError2, // 当前误差
        out;                        // 最后得出的实际增量

    iError = SetSpeed - ActualSpeed; // 计算当前误差  位置
    iError2 = yuzhi_speed - ActualSpeed;

    Increase1 = MOTOR[KP] * (iError) + MOTOR[KD] * (iError - sptr->LastError);

    if (zhidao_flag == 1)
    {
        if (iError2 > 70)
        {
            BangBang_Flag = 1;
            Increase2 += 4500;
        }
        else if (iError2 > 50)
        {
            BangBang_Flag = 1;
            Increase2 += 3500;
        }
        else if (iError2 > 30)
        {
            BangBang_Flag = 1;
            Increase2 += 2500;
        }
        else if (iError2 > 15)
        {
            BangBang_Flag = 1;
            Increase2 += 1500;
        }
    }
    else if (zhidao_flag == 2)
    {
        if (iError2 > 50)
        {
            BangBang_Flag = 1;
            Increase1 += 400;
        }
        else if (iError2 > 30)
        {
            BangBang_Flag = 1;
            Increase1 += 300;
        }
        else if (iError2 > 15)
        {
            BangBang_Flag = 1;
            Increase1 += 200;
        }
    }
    if (jisha_flag)
    {
        if (iError2 < -70)
        {
            BangBang_Flag = 1;
            Increase1 -= 8000;
        }
        else if (iError2 < -50)
        {
            BangBang_Flag = 1;
            Increase1 -= 8000;
        }
        else if (iError2 < -30)
        {
            BangBang_Flag = 1;
            Increase1 -= 8000;
        }
        else if (iError2 < -15)
        {
            BangBang_Flag = 1;
            Increase1 -= 8000;
        }
    }

    if (tingche_flag2 == 1)
    {
        if (iError < -70)
        {
            BangBang_Flag = 1;
            Increase1 -= 5000;
        }
        else if (iError < -50)
        {
            BangBang_Flag = 1;
            Increase1 -= 4000;
        }
        else if (iError < -30)
        {
            BangBang_Flag = 1;
            Increase1 -= 3000;
        }
    }
    else if (poer_flag == 1)
    {
        if (iError < -70)
        {
            BangBang_Flag = 1;
            Increase1 -= 8500;
        }
        else if (iError < -50)
        {
            BangBang_Flag = 1;
            Increase1 -= 7500;
        }
        else if (iError < -30)
        {
            BangBang_Flag = 1;
            Increase1 -= 6000;
        }
        else if (iError < -15)
        {
            BangBang_Flag = 1;
            Increase1 -= 5000;
        }
    }

    out = Increase1;
    return out;
}
long long int PID_Cascade2(PID *sptr, long long int ActualSpeed, long long int SetSpeed)
{
    // 当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
    register long long int iError, iError2, // 当前误差
        out;                        // 最后得出的实际增量

    iError = SetSpeed - ActualSpeed; // 计算当前误差  位置
    iError2 = yuzhi_speed - ActualSpeed;

    Increase2 = MOTOR[KP] * (iError) + MOTOR[KD] * (iError - sptr->LastError);
    if (zhidao_flag == 1)
    {
        if (iError2 > 70)
        {
            BangBang_Flag = 1;
            Increase2 += 4500;
        }
        else if (iError2 > 50)
        {
            BangBang_Flag = 1;
            Increase2 += 3500;
        }
        else if (iError2 > 30)
        {
            BangBang_Flag = 1;
            Increase2 += 2500;
        }
        else if (iError2 > 15)
        {
            BangBang_Flag = 1;
            Increase2 += 1500;
        }
    }
    else if (zhidao_flag == 2)
    {
        if (iError2 > 50)
        {
            BangBang_Flag = 1;
            Increase2 += 400;
        }
        else if (iError2 > 30)
        {
            BangBang_Flag = 1;
            Increase2 += 300;
        }
        else if (iError2 > 15)
        {
            BangBang_Flag = 1;
            Increase2 += 200;
        }
    }
    if (jisha_flag)
    {
        if (iError2 < -70)
        {
            BangBang_Flag = 1;
            Increase2 -= 8000;
        }
        else if (iError2 < -50)
        {
            BangBang_Flag = 1;
            Increase2 -= 8000;
        }
        else if (iError2 < -30)
        {
            BangBang_Flag = 1;
            Increase2 -= 8000;
        }
        else if (iError2 < -15)
        {
            BangBang_Flag = 1;
            Increase2 -= 8000;
        }
    }

    if (tingche_flag2 == 1)
    {
        if (iError < -70)
        {
            BangBang_Flag = 1;
            Increase2 -= 5000;
        }
        else if (iError < -50)
        {
            BangBang_Flag = 1;
            Increase2 -= 4000;
        }
        else if (iError < -30)
        {
            BangBang_Flag = 1;
            Increase2 -= 3000;
        }
    }

    else if (poer_flag == 1)
    {
        if (iError < -70)
        {
            BangBang_Flag = 1;
            Increase2 -= 8500;
        }
        else if (iError < -50)
        {
            BangBang_Flag = 1;
            Increase2 -= 7500;
        }
        else if (iError < -30)
        {
            BangBang_Flag = 1;
            Increase2 -= 6000;
        }
        else if (iError < -15)
        {
            BangBang_Flag = 1;
            Increase2 -= 5000;
        }
    }

    out = Increase2;
    return out;
}

// 增量式PID电机控制
// long long int PID_Realize(PID *sptr, long long int ActualSpeed, long long int SetSpeed)
//{
//     //当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
//     register long long int iError,      //当前误差
//
//     Increase;   //最后得出的实际增量
//     if (iError < -80)
//                    { MOTOR2[KP]=MOTOR[KP]+6;
//                      MOTOR2[KI]=MOTOR[KI]+1;
//                    }
//                    else if (iError < -60)
//                     { MOTOR2[KP]=MOTOR[KP]+3;
//                       MOTOR2[KI]=MOTOR[KI]+0.7;
//                     }
//                    else if (iError < -40&&zhidao_juli>60)
//                     { MOTOR2[KP]=MOTOR[KP]+3;
//                       MOTOR2[KI]=MOTOR[KI]+0.6;
//                     }
//                     else  if (iError < -20&&zhidao_juli>60)
//                     { MOTOR2[KP]=MOTOR[KP]+3;
//                       MOTOR2[KI]=MOTOR[KI]+0.6;
//                     }
//                     else  if (iError < -20&&zhidao_juli>80)
//                      { MOTOR2[KP]=MOTOR[KP]+3;
//                        MOTOR2[KI]=MOTOR[KI]+0.5;
//                      }
//                     else  if (iError < -10&&zhidao_juli>100)
//                      { MOTOR2[KP]=MOTOR[KP]+3;
//                        MOTOR2[KI]=MOTOR[KI]+0.5;
//                      }
//     iError = SetSpeed - ActualSpeed;//计算当前误差
//     Increase = - MOTOR2[KP] * (ActualSpeed - sptr->LastSpeed)    //加速度
//                   + MOTOR2[KI] * iError
//                   + MOTOR2[KD] * (iError - 2 * sptr->LastError + sptr->PrevError);
////
////    Increase = MOTOR[KP] * (iError - sptr->LastError)
////             + MOTOR[KI] * iError
////             + MOTOR[KD] * (iError - 2 * sptr->LastError + sptr->PrevError);
//
//        sptr->PrevError = sptr->LastError;  //更新前次误差
//        sptr->LastError = iError;           //更新上次误差
//        sptr->LastSpeed = ActualSpeed;      //更新上次速度
//
////    if (iError < -70)
////    {
////        BangBang_Flag = 1;
////        Increase -= 100;
////    }
////    else if (iError < -50)
////    {
////        BangBang_Flag = 1;
////        Increase -= 50;
////    }
////    else if (iError < -30)
////    {
////        BangBang_Flag = 1;
////        Increase -= 20;
////    }
////
////    if ((iError > 70) && (BangBang_Flag == 1))
////    {
////        BangBang_Flag = 0;
////        Increase +=  170;
////    }
////    else if (iError > 70)
////    {
////        Increase += 120;
////    }
//
//    return Increase;
//}
////int Turn_Ctl_fuzzy(int point)
////{
////  float turn_err,turn_det;
////  static float turn_err_last;
////  int turn;
////
////  turn_err=point-93;
////  turn_det=turn_err-turn_err_last;
////  turn_err_last=turn_err;
////
////
//////  turn_err=-14.5;
////  turn_det=-3;
//
//  int i,j;
//  /**********隶属度*******/
//  unsigned int NB=-6;
//  unsigned int NM=-4;
//  unsigned int NS=-2;
//  unsigned int ZO=0;
//  unsigned int PS=2;
//  unsigned int PM=4;
//  unsigned int PB=6;
//
//
//
//
////  float  eRule[13]={-6.0,-5.0,-4.0,-3.0,-2.0,-1.0,0.0,1.0,2.0,3.0,4.0,5.0,6.0};   //误差E的模糊论域
////  float  ecRule[7]={-3.0,-2.0,-1.0,0.0,1.0,2.0,3.0}; //误差变化率EC的模糊论域
//
//  float e=turn_err;
//  float ec=turn_det;
//  float es[7];
//  float ecs[7];
//
//  float menbKp;
//  float menbKi;
//  float menbKd;
//
//  float formKp[7][7],formKi[7][7],formKd[7][7];                  //(表7X7)（隶属度表）
//  float sumKp = 0;                         //采用重心法进行解模糊,sum为分母
//  float sumKi = 0;
//  float sumKd = 0;
//  //模糊规则表
//  float FuzzyRuleKp[7][7] = {{PB,PB,PM,PM,PS,ZO,ZO},
//                              {PB,PB,PM,PS,PS,ZO,NS},
//                              {PB,PM,PM,PS,ZO,NS,NS},
//                              {PM,PM,PS,ZO,NS,NM,NM},
//                              {PS,PS,ZO,NS,NS,NM,NB},
//                              {PS,ZO,NS,NM,NM,NM,NB},
//                              {ZO,ZO,NM,NM,NM,NB,NB}};
//
//  /******************计算det_Kp*****************/
//
//  //将偏差从基本论域转换到相应的模糊集论域
//  if(turn_err>=0)
//  {
//    e=turn_err/(93-2)*3;
//  }
//  else
//  {
//    e=turn_err/(184-93)*3;
//  }
//  //偏差变化率从基本论域转换到相应的模糊集论域
//  //turn_det_show=turn_det;
//  ec=turn_det/20*3;
//  /********隶属度计算********************/
//  es[NB] = ufl(e, -3, -1);                       //输入1：偏差E
//  es[NM] = uf(e, -3, -2, 0);
//  es[NS] = uf(e, -3, -1, 1);
//  es[ZO] = uf(e, -2, 0, 2);
//  es[PS] = uf(e, -1, 1, 3);
//  es[PM] = uf(e, 0, 2, 3);
//  es[PB] = ufr(e, 1, 3);
//
//  ecs[NB] = ufl(ec, -3, -1);                    //输入2：偏差变化率Ec
//  ecs[NM] = uf(ec, -3, -2, 0);
//  ecs[NS] = uf(ec, -3, -1, 1);
//  ecs[ZO] = uf(ec, -2, 0, 2);
//  ecs[PS] = uf(ec, -1, 1, 3);
//  ecs[PM] = uf(ec, 0, 2, 3);
//  ecs[PB] = ufr(ec, 1, 3);
//  /******模糊推理 规则的可信度通过取小运算得到*****/
//  for (i = 0; i < 7; i++)
//  {
//      float w,h,r;
//      for (j = 0; j < 7; j++)
//      {
//          h = es[i];
//          r = ecs[j];
//          w = fand(h, r);
//          formKp[i][j] = w;
//          sumKp += w;
//       }
//  }
//  /***************************解模糊采用重心法，以下即求分子**********************************/
//    for (i = 0; i < 7; i++)
//    {
//        for (j = 0; j < 7; j++)
//        {
//            if (FuzzyRuleKp[i][j] == NB) {menbKp += (formKp[i][j] * cufl(formKp[i][j], -6, -2));}
//            else if(FuzzyRuleKp[i][j] == NM) {menbKp += (formKp[i][j] * cuf(formKp[i][j], -6, -4, 0));}
//            else if(FuzzyRuleKp[i][j] == NS) {menbKp += (formKp[i][j] * cuf(formKp[i][j], -6, -2, 2));}
//            else if(FuzzyRuleKp[i][j] == ZO) {menbKp += (formKp[i][j] * cuf(formKp[i][j], -4, 0, 4));}
//            else if(FuzzyRuleKp[i][j] == PS) {menbKp += (formKp[i][j] * cuf(formKp[i][j], -2, 2, 6));}
//            else if(FuzzyRuleKp[i][j] == PM) {menbKp += (formKp[i][j] * cuf(formKp[i][j], 0, 4, 6));}
//            else if(FuzzyRuleKp[i][j] == PB) {menbKp += (formKp[i][j] * cufr(formKp[i][j], 2, 6));}
//        }
//    }
//    det_Kp = (menbKp / sumKp);
//
//
//    float FuzzyRuleKd[7][7] = {{PS,NS,NB,NB,NB,NM,PS},
//                              {PS,NS,NB,NM,NM,NS,ZO},
//                              {ZO,NS,NM,NM,NS,NS,ZO},
//                              {ZO,ZO,NS,NS,NS,NS,ZO},
//                              {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
//                              {PB,NS,PS,PS,PS,PS,PB},
//                              {PB,PM,PM,PM,PS,PS,PB}};
///******模糊推理 规则的可信度通过取小运算得到*****/
//    for (i = 0; i < 7; i++)
//    {
//        float w,h,r;
//        for (j = 0; j < 7; j++)
//        {
//            h = es[i];
//            r = ecs[j];
//            w = fand(h, r);
//            formKd[i][j] = w;
//        sumKd += w;
//         }
//    }
///***************************解模糊采用重心法，以下即求分子**********************************/
//    for (i = 0; i < 7; i++)
//    {
//        for (j = 0; j < 7; j++)
//            {
//                    if (FuzzyRuleKd[i][j] == NB) {menbKd += (formKd[i][j] * cufl(formKd[i][j], -6, -2));}
//                    else if(FuzzyRuleKd[i][j] == NM) {menbKd += (formKd[i][j] * cuf(formKd[i][j], -6, -4, 0));}
//                    else if(FuzzyRuleKd[i][j] == NS) {menbKd += (formKd[i][j] * cuf(formKd[i][j], -6, -2, 2));}
//                    else if(FuzzyRuleKd[i][j] == ZO) {menbKd += (formKd[i][j] * cuf(formKd[i][j], -4, 0, 4));}
//                    else if(FuzzyRuleKd[i][j] == PS) {menbKd += (formKd[i][j] * cuf(formKd[i][j], -2, 2, 6));}
//                    else if(FuzzyRuleKd[i][j] == PM) {menbKd += (formKd[i][j] * cuf(formKd[i][j], 0, 4, 6));}
//                    else if(FuzzyRuleKd[i][j] == PB) {menbKd += (formKd[i][j] * cufr(formKd[i][j], 2, 6));}
//            }
//    }
//
//    det_Kd = (menbKd / sumKd);
//
//    turn=stree_center+(int)(turn_err*(S_D5[Set][KP]+det_Kp*10.0f)+turn_det*(S_D5[Set][KD]+det_Kd*30.0f));
//
//                    if(turn>=stree_max)   turn= stree_max;
//                    if(turn<=stree_min)   turn = stree_min;
//   // turn_p_show=turn_p+det_Kp;
//   // turn_d_show=turn_d+det_Kd;
//    return turn;
//}
//
//
//
//
////高斯函数隶属度
// float gaussmf(float x,float u,float a)
//{
//    return 0.39894228/a*exp(-(x-u)*(x-u)/(2*a*a));
// }
//
////高斯函数隶属度
// float gaussmf_ec(float x,float u)
//{
//    return exp(-(x-u)*(x-u)*2);
// }
//
////三角形函数隶属度
// float cuf_w(float x,float u)
//{
//   if(x>=u&&x<=u+2)
//   {
//    return -0.5*(x-u)+1;
//   }
//   else if(x<u&&x>=u-2)
//   {
//    return 0.5*(x-u)+1;
//   }
//   else
//   {
//     return 0;
//   }
// }
//
///****************三角形反模糊化处理(最大隶属度法)**********************/
///**************求隶属度（三角形）模糊化处理***************/
// float uf( float x, float a, float b, float c)
//{
//     if(x<=a)
//         return 0;
//     else if((a<x)&&(x<=b))
//         return (x-a)/(b-a);
//     else if((b<x)&&(x<=c))
//         return (c-x)/(c-b);
//     else if(x>c)
//         return 0;
//      return 0;
// }
///****************三角形反模糊化处理(最大隶属度法)**********************/
// float cuf( float x, float a, float b, float c)
//{
//     float y,z;
//     z=(b-a)*x+a;
//     y=c-(c-b)*x;
//     return (y+z)/2;
// }
///*****************梯形(左)求隶属度 模糊化*******************/
// float ufl( float x, float a, float b)
//{
//     if(x<=a)
//         return 1;
//     else if((a<x)&&(x<=b))
//         return (b-x)/(b-a);
//     else if(x>b)
//         return 0;
//     return 0;
// }
//
///*******************梯形反模糊化***********************/
// float cufl( float x, float a, float b)
//{
//     return (b-(b-a)*x);
// }
//
//
///*****************梯形(右)求隶属度 模糊化*******************/
// float ufr( float x, float a, float b)
//{
//     if(x<=a)
//         return 0;
//     if((a<x)&&(x<b))
//         return (x-a)/(b-a);
//     if(x>=b)
//         return 1;
//       return 0;
// }
///*******************梯形反模糊化***********************/
// float cufr( float x, float a, float b)
//{
//     return (b-a)*x +a;
// }
//
////求交集
// float fand(float a,float b)
//{
//   return (a<b)?b:a;
// }
//
// float Abs(float a)
//{
//   if(a>0) return a;
//   else return (0-a);
//
// }
PID2 fuzzy(float e, float ec) // e 是目标值和反馈值的误差 ec是误差变化率(误差e的微分)//FuzzyPidMotor_OpenPwm
{
    float etemp, ectemp;
    float eLefttemp, ecLefttemp;
    float eRighttemp, ecRighttemp;

    int eLeftIndex, ecLeftIndex;
    int eRightIndex, ecRightIndex;
    PID2 fuzzy_PID;

    etemp = e > 3.0 ? 0.0 : (e < -3.0 ? 0.0 : (e >= 0.0 ? (e >= 2.0 ? 2.5 : (e >= 1.0 ? 1.5 : 0.5)) : (e >= -1.0 ? -0.5 : (e >= -2.0 ? -1.5 : (e >= -3.0 ? -2.5 : 0.0)))));

    eLeftIndex = (int)e;
    eRightIndex = eLeftIndex;
    eLeftIndex = (int)((etemp - 0.5) + 3); //[-3,3] -> [0,6]
    eRightIndex = (int)((etemp + 0.5) + 3);

    eLefttemp = etemp == 0.0 ? 0.0 : ((etemp + 0.5) - e);
    eRighttemp = etemp == 0.0 ? 0.0 : (e - (etemp - 0.5));

    ectemp = ec > 3.0 ? 0.0 : (ec < -3.0 ? 0.0 : (ec >= 0.0 ? (ec >= 2.0 ? 2.5 : (ec >= 1.0 ? 1.5 : 0.5)) : (ec >= -1.0 ? -0.5 : (ec >= -2.0 ? -1.5 : (ec >= -3.0 ? -2.5 : 0.0)))));

    ecLeftIndex = (int)((ectemp - 0.5) + 3); //[-3,3] -> [0,6]
    ecRightIndex = (int)((ectemp + 0.5) + 3);

    ecLefttemp = ectemp == 0.0 ? 0.0 : ((ectemp + 0.5) - ec);
    ecRighttemp = ectemp == 0.0 ? 0.0 : (ec - (ectemp - 0.5));

    /*************************************反模糊*************************************/
    fuzzy_PID.Kp = (eLefttemp * ecLefttemp * fuzzyRuleKp[ecLeftIndex][eLeftIndex] + eLefttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eLeftIndex] + eRighttemp * ecLefttemp * fuzzyRuleKp[ecLeftIndex][eRightIndex] + eRighttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eRightIndex]);

    fuzzy_PID.Ki = (eLefttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eLeftIndex] + eLefttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eLeftIndex] + eRighttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eRightIndex] + eRighttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eRightIndex]);

    fuzzy_PID.Kd = (eLefttemp * ecLefttemp * fuzzyRuleKd[ecLeftIndex][eLeftIndex] + eLefttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eLeftIndex] + eRighttemp * ecLefttemp * fuzzyRuleKd[ecLeftIndex][eRightIndex] + eRighttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eRightIndex]);
    return fuzzy_PID;
}

float FuzzyPid_Out(float tar, float cur) // 目标值 , 实际值
{
    // float tar = 0,cur = 0;                //目标值 , 实际值
    float e = 0;                                                     // 误差e 误差变化率ec(误差e的微分)  系统死区设置量 不一定为零
                                                                     //  static PID pid= {0.6, 0.082, 0.12};      //赋予初值kp，ki，kd
    static PID2 pid = {5.0, 0.05, 0};                                //-2.8
    static float sumOUT = 0, pwmOUT = 0, old1 = 0, old2 = 0, ec = 0; // 累加偏差 old1 old2
    PID2 OUT = {0, 0, 0};
    e = tar - cur; // 实际值 - 目标值
    ec = old1 - e;
    OUT = fuzzy(e, ec); // 模糊控制调整  kp，ki，kd

    pwmOUT = (pid.Kp + OUT.Kp) * e + (pid.Ki + OUT.Ki) * old1 + (pid.Kd + OUT.Kd) * old2;
    old2 = old1;
    old1 = e;
    sumOUT += pwmOUT;
    return sumOUT; // 最终输出值
}
