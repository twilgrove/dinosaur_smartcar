#ifndef __CORE_H__
#define __CORE_H__

#include "head_system.h"

#include "path.h"

#include "data_process.h"
#include "image_process.h"
#include "data_config.h"

#include "pwm_ctrl.h"
#include "GPIO.h"
#include "key.h"
#include "pid.h"
#include "encoder.h"
#include "tty.h"
#include "udp.h"
#include "memory.h"
/* ----------------------------------------宏定义---------------------------------------- */
#define PROGRAM_NAME "Smart_Car"

#define WHEEL_MAX_PWM 20000
#define WHEEL_MIN_PWM 0
#define SPEED_DEADBAND 5 // 目标速度死区

#define SERVO_MAX_PWM 1600000
#define SERVO_MIN_PWM 1300000

#define MAX_OUTPUT_LIMIT(x, max) ((x) > (max) ? (max) : (x)) // 输出限幅
#define MIN_OUTPUT_LIMIT(x, min) ((x) < (min) ? (min) : (x)) // 输出限幅
/* ----------------------------------------全局变量---------------------------------------- */
extern std::atomic<bool> running;

/* 内存监控 */
extern MemoryMonitor memory_monitor;

/* 决策及处理 */
extern ImgProcess my_img_process; // 图像处理
extern Judge my_judge;            // 赛道决策
extern SYNC my_sync;              // 同步

// 数据结构体指针定义
extern Img_Store Img_Store_c;
extern Img_Store *Img_Store_p; // 图像存储与指针
extern Function_EN Function_EN_c;
extern Function_EN *Function_EN_p; // 功能使能与指针
extern Data_Path Data_Path_c;
extern Data_Path *Data_Path_p; // 赛道数据与指针

extern JSON_FunctionConfigData JSON_functionConfigData;
extern JSON_TrackConfigData JSON_trackConfigData;

/* 相机 */
extern cv::VideoCapture Camera;

/* 视频发送 */
extern udp sender;
extern bool sender_ready;
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

/* ----------------------------------------函数声明---------------------------------------- */
void init();                     // 初始化
void run();                      // 准备运行
void close();                    // 关闭程序
void project_manage(int signum); // 程序信息
void reset(bool flag);           // 重置程序

void opencv_thread();           // opencv线程
void fans_pwm_thread();         // 负压风扇控制线程
void right_pid_pwm_thread();    // 右轮控制线程
void left_pid_pwm_thread();     // 左轮控制线程
void servo_pid_pwm_thread();    // 舵机控制线程
void imu_thread();              // 陀螺仪数据处理线程
void gpio_thread();             // GPIO控制线程
void debugi_thread();           // 调试输入线程
void debugo_thread();           // 调试输出线程
void car_main_control_thread(); // 小车控制线程

#endif // __MAIN_H__