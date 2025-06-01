#include "image_deal.h"
#include "thread.h"
#include "isr.h"
#include "key_board.h"
#include "PID.h"
#include "my_control.h"
#include "traffic_circle.h"
#include "headfile.h"
#include "key_board.h"
/* ----------------------------------------全局变量---------------------------------------- */

/* 串口 */
SerialPort tty("/dev/ttyS1", B115200);
uint8_t deta[8];
float value = 0;
uint32_t value_int = 0;

/* 编码器,10ms更新一次 */
ENCODER left_encoder(0, 51);
float l_now = 0;
ENCODER right_encoder(3, 50);
float r_now = 0;

/* 电机速度 ：0-200*/
/* 左电机：频率20-50khz，周期20000-50000ns，占空比0-20000ns*/
uint32_t lp_duty = 0;
float l_target = 0; // 电机速度 ：0-200
GPIO l_pin(72, "out", 0);
pwm_ctrl lp(2, 0, 20000, lp_duty, "left_motor");
pid lp_pid(pid::Mode::INCREMENT, 40, 20, 0, 1500, WHEEL_MAX_PWM, WHEEL_MIN_PWM);

/* 右电机：频率20-50khz，周期20000-50000ns，占空比0-20000ns*/
uint32_t rp_duty = 0;
float r_target = 0; // 电机速度 ：0-200
GPIO r_pin(73, "out", 0);
pwm_ctrl rp(1, 0, 20000, rp_duty, "right_motor");
pid rp_pid(pid::Mode::INCREMENT, 40, 20, 0, 1500, WHEEL_MAX_PWM, WHEEL_MIN_PWM);

/* 舵机：频率50hz，周期20,000,000ns，占空比1300,000-1,600,000ns*/
uint32_t sp_duty = 1522000;
float s_target = 0;
pwm_ctrl sp(8, 6, 20000000, sp_duty, "servo");
pid sp_pid(pid::Mode::POSITION, 1, 0, 0, 1000, SERVO_MAX_PWM-MIDO_sp, SERVO_MIN_PWM-MIDO_sp);

/* 按键 */
Key key1(16, Key::up);
Key key2(15, Key::up);
Key key3(14, Key::up);
Key key4(13, Key::up);

uint8_t key_1_value = 0;
uint8_t key_2_value = 0;
uint8_t key_3_value = 0;
uint8_t key_4_value = 0;

/* 开关，低有效 */
GPIO switch1(20, "in");
GPIO switch2(21, "in");

bool switch1_value = 1;
bool switch2_value = 1;

/* 蜂鸣器，高有效 */
GPIO buzzer(12, "out", 0);

/* 陀螺仪 */
IMUFilter imu_filter;


cv::Mat get_color;
cv::Mat get_image;
