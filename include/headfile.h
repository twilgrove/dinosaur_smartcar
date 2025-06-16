#ifndef _headfile_h
#define _headfile_h

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <utility>
#include <atomic>
#include <vector>
#include <cmath>
#include <sys/time.h>

#include "opencv2/opencv.hpp"

#include "udp.h"
#include "pwm_ctrl.h"
#include "GPIO.h"
#include "key.h"
#include "pid.h"
#include "encoder.h"
#include "tty.h"
#include "IMUFilter.h"
#include "timer.h"
#include "zf_common_headfile.h"

#include "my_control.h"
#include "traffic_circle.h"
#include "isr.h"
#include "key_board.h"
#include "PID.h"
#include "camera.h"
#include "image_deal.h"

#include "john.h"

#include "hardware_control.h"

/* ----------------------------------------配置宏定义---------------------------------------- */
#define PROGRAM_NAME "Smart_Car"

#define KEY1_START_EN 1 // 按键启动使能
#define VOFA_DEBUG_EN 0 // vofa调试使能

#define OPENCV_THREAD_PERIOD 10   // opencv线程周期(ms)
#define CAR_THREAD_PERIOD 20      // 算法控制线程周期(ms)
#define HARDWARE_THREAD_PERIOD 2  // 硬件控制线程周期(ms)
#define H_DEBUG_THREAD_PERIOD 50  // 高速调试线程周期(ms)
#define L_DEBUG_THREAD_PERIOD 500 // 低速调试线程周期(ms)
#define IO_THREAD_PERIOD 10       // IO线程周期(ms)

#define UDP_PORT 8080           // 图传接收端口
#define DST_IP "192.168.43.180" // 图传接收IP
#define MAX_PACKET_SIZE 1024    // 图传接收最大包大小

#define WHEEL_MAX_PLUS_ns 16000 // 电机最大值(20000)
#define WHEEL_MIN_PLUS_ns 0     // 电机最小值
#define WHEEL_SPEED_DEADBAND 3  // 电机速度死区
#define WHEEL_SPEED_MAX 100     // 电机速度最大值
#define WHEEL_SPEED_PID_KP 100  // 电机速度PID增益系数

#define SERVO_MID_PLUS_ns 1540000 // 舵机中值
#define SERVO_MAX_PLUS_ns 1680000 // 舵机最大值(左转)
#define SERVO_MIN_PLUS_ns 1391000 // 舵机最小值(右转)

/* ----------------------------------------函数宏定义---------------------------------------- */
#define MAX_OUTPUT_LIMIT(x, max) ((x) > (max) ? (max) : (x)) // 输出限幅
#define MIN_OUTPUT_LIMIT(x, min) ((x) < (min) ? (min) : (x)) // 输出限幅

/* ----------------------------------------函数声明---------------------------------------- */
void init();
void img_process();
void car_main_control_thread();
void h_debug_thread();
void hardware_control();
void l_debug_thread();
void IO_thread();

void project_manage(int signum);
void reset(bool flag);
void Test_period(const char *threadName);
void Update_time();
void Update_runing_state();
void Update_motor();
void Update_ips();
void Update_servo();
void Show_image();
/* ----------------------------------------结构体声明---------------------------------------- */
typedef struct
{
    bool program_running; // 程序运行状态
    bool car_running;     // 小车运行状态
    bool Camera_running;  // 摄像头运行状态
    uint8_t cam_frame;    // 摄像头帧率
    bool IMU_running;     // IMU运行状态

    bool IMG_send;    // 图传状态
    bool IMG_display; // 图显状态

    struct timeval start; // 运行开始时间
    struct timeval now;   // 当前时间
    double total_seconds; // 总秒数
    int minutes;          // 分钟
    double seconds;       // 秒
} car_state;

/* --------------------------------------全局变量声明---------------------------------------- */

extern car_state car;
extern SerialPort tty;
extern uint8_t tty_data[8];
extern float tty_value;
extern uint32_t tty_value_int;

extern ENCODER left_encoder;
extern float l_now;
extern ENCODER right_encoder;
extern float r_now;

extern uint32_t lp_duty;
extern float l_target;
extern GPIO l_pin;
extern pwm_ctrl lp;
extern pid lp_pid;

extern uint32_t rp_duty;
extern float r_target;
extern GPIO r_pin;
extern pwm_ctrl rp;
extern pid rp_pid;

extern uint32_t lfs_duty;
extern pwm_ctrl l_fs;
extern uint32_t rfs_duty;
extern pwm_ctrl r_fs;

extern uint32_t sp_duty;
extern uint32_t last_sp_duty;
extern pwm_ctrl sp;
extern pid sp_pid;

extern Key key1;
extern Key key2;
extern Key key3;
extern Key key4;

extern uint8_t key_1_value;
extern uint8_t key_2_value;
extern uint8_t key_3_value;
extern uint8_t key_4_value;

extern GPIO switch1;
extern GPIO switch2;

extern bool switch1_value;
extern bool switch2_value;

extern GPIO buzzer;

extern IMUFilter imu_filter;

extern cv::VideoCapture Camera;

extern UdpSender ImgSender;
extern std::mutex image_mutex;
extern cv::Mat image_to_send;
extern cv::Mat image_to_show;
extern unsigned char image_show[90][240];
extern cv::Mat get_image;
extern cv::Mat get_color;

/*john 实时检测*/
extern unsigned int opencv_v, control_v, sp_v;
extern uint8_t wan_flag;
extern uint16_t wan_num;
extern uint16_t line_num;
/*john 实时检测*/

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

extern int Point_last1;

extern int car_flag;

#endif
