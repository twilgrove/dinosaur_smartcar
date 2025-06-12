/*
 * keyboard.c
 *
 *  Created on: 2022年5月20日
 *      Author: admin
 */



#include "headfile.h"

unsigned long int write_buf;
unsigned long int huihuan_sd=320;
int huan_bankuan=0,com_bankuan=0;
int zhidao_jia=0;
unsigned long int por_sd=0,yanhuan_sd=0,huan_top=0,com_top=0,por_top=0;
unsigned int junsu=1;
unsigned int  caxie=0;
unsigned long int  power;
unsigned int KDD=26;
unsigned int xiaozhi=0;
int gou_Guo2=0;
unsigned long int  line_number=0,line_number_previous=0;
extern unsigned long int expose_time;
extern unsigned int ruku_zuobiao_hang;
extern unsigned int ruku_zuobiao_lie;
extern unsigned int p_chasu;
extern int Set_Speed2;
unsigned int dispagenum=0,dispagenum_last=0;  //显示内容
unsigned int paraadjnum=9;      //参数调节项目
int contin=1;

unsigned int c_l=2,c_r=1; //左大环2 左小环1 右小环1 右大环2
unsigned long int sd_fast=900;    //上限速度
unsigned long int clc_sd=700;   //入环岛速度
unsigned int status;



extern int th_y;
