/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编   写：龙邱科技
@邮   箱：chiusir@163.com
@编译IDE：Linux 环境、VSCode_1.93 及以上版本、Cmake_3.16 及以上版本
@使用平台：龙芯2K0300久久派和北京龙邱智能科技龙芯久久派拓展板
@相关信息参考下列地址
    网      站：http://www.lqist.cn
    淘 宝 店 铺：http://longqiu.taobao.com
    程序配套视频：https://space.bilibili.com/95313236
@软件版本：V1.0 版权所有，单位使用请先联系授权
@参考项目链接：https://github.com/AirFortressIlikara/ls2k0300_peripheral_library

@修改日期：2025-02-26
@修改内容：
@注意事项：注意查看路径的修改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_SGP18T_TFTSPI_H__
#define __LQ_SGP18T_TFTSPI_H__	

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "stdint.h"
#include "LQ_HW_GPIO.hpp"
#include "LQ_HW_SPI.hpp"

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

//龙邱TFT-SPI模块管脚顺序
#define TFTSPI_CS       63 // CS管脚 默认拉低，可以不用
#define TFTSPI_SCK      60 // SPI SCK管脚
#define TFTSPI_SDI      62 // SPI MOSI管脚
#define TFTSPI_DC       48 // D/C管脚
#define TFTSPI_RST      49 // RESET管脚

#define TFTSPI_CS_H     TFTSPI_CS_fd.SetGpioValue(1);      /*!< CS管脚 */
#define TFTSPI_SCK_H    TFTSPI_SCK_fd.SetGpioValue(1);     /*!< SCL管脚 */
#define TFTSPI_SDI_H    TFTSPI_SDI_fd.SetGpioValue(1);     /*!< SDI管脚 */
#define TFTSPI_DC_H     TFTSPI_DC_fd.SetGpioValue(1);      /*!< DC管脚 */
#define TFTSPI_RST_H    TFTSPI_RST_fd.SetGpioValue(1);     /*!< RST管脚 */

#define TFTSPI_CS_L     TFTSPI_CS_fd.SetGpioValue(0);     /*!< CS管脚 */
#define TFTSPI_SCK_L    TFTSPI_SCK_fd.SetGpioValue(0);    /*!< SCL管脚 */
#define TFTSPI_SDI_L    TFTSPI_SDI_fd.SetGpioValue(0);    /*!< SDI管脚 */
#define TFTSPI_DC_L     TFTSPI_DC_fd.SetGpioValue(0);     /*!< DC管脚 */
#define TFTSPI_RST_L    TFTSPI_RST_fd.SetGpioValue(0);    /*!< RST管脚 */

/*******************接口定义******************************/

#define TFT18W        162
#define TFT18H        132

#define u16RED		0xf800
#define u16GREEN	0x07e0
#define u16BLUE	    0x001f
#define u16PURPLE	0xf81f
#define u16YELLOW	0xffe0
#define u16CYAN	    0x07ff 		//蓝绿色
#define u16ORANGE	0xfc08
#define u16BLACK	0x0000
#define u16WHITE	0xffff

/* TFT18初始化 */
void TFTSPI_Init(unsigned char type);
/* 写命令 */
void TFTSPI_Write_Cmd(unsigned char cmd);
/* 写字节 */
void TFTSPI_Write_Byte(unsigned char dat);
/* 写半字 */
void TFTSPI_Write_Word(unsigned short dat);
/* 重新定位输入信息位置 */
void TFTSPI_Set_Pos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);
/* 填充指定区域 */
void TFTSPI_Fill_Area(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color);
/* 全屏显示单色画面 */
void TFTSPI_CLS(unsigned short color);
/* 画点 */
void TFTSPI_Draw_Dot(unsigned char x,unsigned char y,unsigned short color_dat);
/* 填充矩形区域 */
void TFTSPI_Draw_Part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
/* 画矩形边框 */
void TFTSPI_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
/* 画圆 */
void TFTSPI_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat);
/* 画线 */
void TFTSPI_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
/* 显示图片 */
void TFTSPI_Show_Pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,const unsigned char *ppic);
/* 液晶字符串输出(6*8字体) */
void TFTSPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
/* 液晶字符串输出(8*8字体) */
void TFTSPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
/* 液晶字符串输出(8*16字体) */
void TFTSPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
/* 液晶汉字字符串输出(16*16字体) */
void TFTSPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color);
/* 显示图片 */
void TFTSPI_Show_Pic2(unsigned char xs,unsigned char ys,unsigned char w,unsigned char h,const unsigned char *ppic) ;
/* TFT18屏 unsigned char 灰度数据显示 */
void TFTSPI_Road( unsigned char wide_start, unsigned char high_start,unsigned char high, unsigned char wide, unsigned char *Pixle);
/* TFT18屏 unsigned char 二值化数据显示 */
void TFTSPI_BinRoad( u8 wide_start, u8 high_start, u8 high, u8 wide, u8 *Pixle);
/* 功能测试 */
void Test_TFT18(void);

#endif
