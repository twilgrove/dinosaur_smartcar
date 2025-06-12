#include "headfile.h"
/* ----------------------------------------全局变量---------------------------------------- */

/* 整车状态 */
bool running = 1;

/* 图传 */
UdpSender ImgSender;
cv::Mat image_to_send;
std::mutex image_mutex;

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
/* 左电机：频率50khz，周期20000ns，脉冲宽度0-20000ns*/
uint32_t lp_duty = 0;
float l_target = 0; // 电机速度 ：0-200
GPIO l_pin(72, "out", 0);
pwm_ctrl lp(2, 0, 20000, lp_duty, "left_motor");
pid lp_pid(Mode::INCREMENT, 40, 20, 0, 1500, WHEEL_MAX_PLUS_ns, WHEEL_MIN_PLUS_ns);

/* 右电机：频率50khz，周期20000ns，脉冲宽度0-20000ns*/
uint32_t rp_duty = 0;
float r_target = 0; // 电机速度 ：0-200
GPIO r_pin(73, "out", 0);
pwm_ctrl rp(1, 0, 20000, rp_duty, "right_motor");
pid rp_pid(Mode::INCREMENT, 40, 20, 0, 1500, WHEEL_MAX_PLUS_ns, WHEEL_MIN_PLUS_ns);

/* 舵机：频率200hz，周期5,000,000ns，脉冲宽度1300,000-1,600,000ns*/
uint32_t sp_duty = SERVO_MID_PLUS_ns;
uint32_t last_sp_duty = SERVO_MID_PLUS_ns;
pwm_ctrl sp(8, 6, 5000000, sp_duty, "servo");
pid sp_pid(Mode::POSITION, 1, 0, 0, 1000, SERVO_MAX_PLUS_ns - SERVO_MID_PLUS_ns, SERVO_MIN_PLUS_ns - SERVO_MID_PLUS_ns);

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

/*图像*/
cv::VideoCapture Camera;
cv::Mat get_color;
cv::Mat get_image;

/*实时检测*/
unsigned int opencv_v, control_v, sp_v;
uint8_t wan_flag = 0;
uint16_t wan_num = 0;
uint16_t line_num = 0;