#ifndef __zf_device_ips200_fb_h__
#define __zf_device_ips200_fb_h__

#include "zf_common_typedef.h"

#define IPS200_DEFAULT_PENCOLOR (RGB565_RED)  // 默认的画笔颜色
#define IPS200_DEFAULT_BGCOLOR (RGB565_WHITE) // 默认的背景颜色

void ips200_clear(void);                                                                               // 清屏
void ips200_full(const uint16 color);                                                                  // 全屏填充颜色
void ips200_draw_point(uint16 x, uint16 y, const uint16 color);                                        // 画点
void ips200_draw_line(uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color); // 画线

void ips200_show_char(uint16 x, uint16 y, const char dat);                               // 显示字符
void ips200_show_string(uint16 x, uint16 y, const char dat[]);                           // 显示字符串
void ips200_show_int(uint16 x, uint16 y, const int32 dat, uint8 num);                    // 显示整数
void ips200_show_uint(uint16 x, uint16 y, const uint32 dat, uint8 num);                  // 显示无符号整数
void ips200_show_float(uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum); // 显示浮点数

void ips200_show_gray_image(uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height); // 显示灰度图像

void ips200_init(const char *path); // 初始化屏幕

#endif