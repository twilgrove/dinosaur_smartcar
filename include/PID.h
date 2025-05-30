/*
 * PID.h
 *
 *  Created on: 2022年5月21日
 *      Author: admin
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_


#include "headfile.h"

#define KP 0
#define KI 1
#define KD 2
#define KT 3
 extern   float Kp1;      //动态P
typedef struct PID
{
    long SumError;  //误差累计
    long long int LastError;    //Error[-1]
    long long int PrevError;    //Error[-2]
    long long int LastSpeed;    //Speed[-1]
} PID;

extern PID S_D5_PID, MOTOR_PID,MOTOR2_PID; //定义舵机和电机的PID参数结构体
extern long long int S_D5[10][4];
//位置式PID参数初始化
void PlacePID_Init(PID *sptr);

//增量式PID参数初始化
void IncPID_Init(PID *sptr);

//位置式PID舵机控制
long long int PlacePID_Control(PID *sprt, long long int NowPiont, long long int SetPoint);

//增量式串级PID控制
long long int PID_Cascade(PID *sptr, long long int ActualSpeed, long long int SetSpeed);
long long int PID_Cascade2(PID *sptr, long long int ActualSpeed, long long int SetSpeed);

//增量式PID控制
long long int PID_Realize(PID *sptr, long long int ActualSpeed, long long int SetSpeed);
int Turn_Ctl_fuzzy(int point);///模糊pid
float Abs(float a);
float fand(float a,float b);
float cufr( float x, float a, float b);
float ufr( float x, float a, float b);
float cufl( float x, float a, float b);
float ufl( float x, float a, float b);
float cuf( float x, float a, float b, float c);
float uf( float x, float a, float b, float c);
float cuf_w(float x,float u);
float gaussmf_ec(float x,float u);
float gaussmf(float x,float u,float a);
/*******mohu**********/
#define NL   -3
#define NM   -2
#define NS   -1
#define ZE   0
#define PS   1
#define PM   2
#define PL   3

//注意：static PID pid= {0, 0, 0};  需要自己赋值

static const float fuzzyRuleKp[7][7]={
    PL, PL, PM, PM, PS, PS, ZE,
    PL, PL, PM, PM, PS, ZE, ZE,
    PM, PM, PM, PS, ZE, NS, NM,
    PM, PS, PS, ZE, NS, NM, NM,
    PS, PS, ZE, NS, NS, NM, NM,
    ZE, ZE, NS, NM, NM, NM, NL,
    ZE, NS, NS, NM, NM, NL, NL
};

static const float fuzzyRuleKi[7][7]={
    NL, NL, NL, NM, NM, ZE, ZE,
    NL, NL, NM, NM, NS, ZE, ZE,
    NM, NM, NS, NS, ZE, PS, PS,
    NM, NS, NS, ZE, PS, PS, PM,
    NS, NS, ZE, PS, PS, PM, PM,
    ZE, ZE, PS, PM, PM, PL, PL,
    ZE, ZE, PS, PM, PL, PL, PL
};

static const float fuzzyRuleKd[7][7]={
    PS, PS, ZE, ZE, ZE, PL, PL,
    NS, NS, NS, NS, ZE, NS, PM,
    NL, NL, NM, NS, ZE, PS, PM,
    NL, NM, NM, NS, ZE, PS, PM,
    NL, NM, NS, NS, ZE, PS, PS,
    NM, NS, NS, NS, ZE, PS, PS,
    PS, ZE, ZE, ZE, ZE, PL, PL
};

typedef struct{
    float Kp;
    float Ki;
    float Kd;
}PID2;

#define FuzzyPidMotor_OpenPwm 3500 // 电机死区
#define FuzzyPidTarge_Error 0 // 目标值允许误差

PID2 fuzzy(float e,float ec); // 模糊PID自整定函数
float FuzzyPid_Out(float tar,float cur);  // 目标值 , 实际值
void motor_Fuzzypid_control(float PID_OUT);

extern float pwmout;
/*****/


#endif /* CODE_PID_H_ */
