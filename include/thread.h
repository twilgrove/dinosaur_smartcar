#ifndef _thread_h
#define _thread_h

#include <cmath>
#include <mutex>
#include <chrono>
#include <iomanip>  // 需要包含这个头文件

#include "headfile.h"

extern UdpSender g_udp_sender; // 全局声明
extern std::mutex image_mutex; // 定义一个互斥锁
extern cv::Mat image_to_send;  // 需要在多个线程之间共享的图像数据


void debugi_thread();

#endif
