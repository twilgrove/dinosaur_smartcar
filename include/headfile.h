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
#include "udp.h"
#include "zf_common_headfile.h"
#include "pwm_ctrl.h"
#include "GPIO.h"
#include "key.h"
#include "pid.h"
#include "encoder.h"
#include "tty.h"
#include "IMUFilter.h"


/* ----------------------------------------宏定义---------------------------------------- */
#define PROGRAM_NAME "Smart_Car"

#define WHEEL_MAX_PWM 20000
#define WHEEL_MIN_PWM 0
#define SPEED_DEADBAND 3 // 目标速度死区

#define MIDO_sp 1522000
#define SERVO_MAX_PWM 1700000-50000
#define SERVO_MIN_PWM 1346000+50000

#define MAX_OUTPUT_LIMIT(x, max) ((x) > (max) ? (max) : (x)) // 输出限幅
#define MIN_OUTPUT_LIMIT(x, min) ((x) < (min) ? (min) : (x)) // 输出限幅
/* ----------------------------------------全局变量---------------------------------------- */

/* 串口 */
extern SerialPort tty;
extern uint8_t deta[8];
extern float value;
extern uint32_t value_int;

/* 编码器,10ms更新一次 */
extern ENCODER left_encoder;
extern float l_now;
extern ENCODER right_encoder;
extern float r_now;

/* 电机速度 ：0-200*/
/* 左电机：频率20-50khz，周期20000-50000ns，占空比0-20000ns*/
extern uint32_t lp_duty;
extern float l_target; // 电机速度 ：0-200
extern GPIO l_pin;
extern pwm_ctrl lp;
extern pid lp_pid;

/* 右电机：频率20-50khz，周期20000-50000ns，占空比0-20000ns*/
extern uint32_t rp_duty;
extern float r_target; // 电机速度 ：0-200
extern GPIO r_pin;
extern pwm_ctrl rp;
extern pid rp_pid;

/* 舵机：频率50hz，周期20,000,000ns，占空比1300,000-1,600,000ns*/
extern uint32_t sp_duty;
extern float s_target;
extern pwm_ctrl sp;
extern pid sp_pid;

/* 按键 */
extern Key key1;
extern Key key2;
extern Key key3;
extern Key key4;

extern uint8_t key_1_value;
extern uint8_t key_2_value;
extern uint8_t key_3_value;
extern uint8_t key_4_value;

/* 开关，低有效 */
extern GPIO switch1;
extern GPIO switch2;

extern bool switch1_value;
extern bool switch2_value;

/* 蜂鸣器，高有效 */
extern GPIO buzzer;

/* 陀螺仪 */
extern IMUFilter imu_filter;

/* 赛道信息 */
extern int Left_Line_New[70], Right_Line_New[70];   // 边界补线的坐标
extern int Left_Line_New2[70], Right_Line_New2[70]; // 边界补线的坐标
extern int center_th[70];
extern int Width_Min;
extern int Left_Add_Start, Left_Add_End;
extern int Right_Add_Start, Right_Add_End;
extern float Add_Slope, Left_Last_Slope, Right_Last_Slope;
extern int temp_r;
extern unsigned int Foresight_Left; ////左侧最小赛道宽度
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


extern cv::Mat get_image;
extern cv::Mat get_color;


void init();
void car_main_control_thread();
void project_manage(int signum);

#endif
