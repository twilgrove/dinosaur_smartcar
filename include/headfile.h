
#ifndef _headfile_h
#define _headfile_h


#include <math.h>
#include <string.h>
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

#include "opencv2/opencv.hpp"
#include "image_deal.h"
#include "key_board.h"
#include "udp.h"



extern int Left_Line_New[70], Right_Line_New[70];   // 边界补线的坐标
extern int Left_Line_New2[70], Right_Line_New2[70]; // 边界补线的坐标
extern int center_th[70];
extern int Width_Min;
extern int Left_Add_Start , Left_Add_End ;
extern int Right_Add_Start, Right_Add_End ;
extern float Add_Slope, Left_Last_Slope, Right_Last_Slope;
extern int temp_r;
extern unsigned int Foresight_Left;////左侧最小赛道宽度
extern unsigned int Foresight_Right;
extern int white_num_col_min;
extern int qvlv_quanju_right, qvlv_quanju_left, qulv_jinduan_right, qulv_jinduan_left, qulv_yuandaun_right, qulv_yuandaun_left;





extern unsigned char image_use[70][188];
extern unsigned int s;
extern unsigned int car_gogogo;

extern int running;
extern cv::VideoCapture Camera;

extern UdpSender g_udp_sender; // 全局声明
extern std::mutex image_mutex; // 定义一个互斥锁
extern cv::Mat image_to_send;  // 需要在多个线程之间共享的图像数据


void init();
void car_main_control_thread();


#endif

