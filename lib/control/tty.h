#ifndef TTY_H
#define TTY_H

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <stdarg.h>
#include <sstream>
#include <iomanip>
class SerialPort
{
public:
    SerialPort(const std::string &device, speed_t baud_rate);
    ~SerialPort();

    void printf(const char *fmt, ...);           // 写入数据
    bool readData(uint8_t *data, size_t length); // 非阻塞读取数据
    bool readString(std::string &data);          // 非阻塞读取字符串
    bool readHexString(std::string &hex_data);   // 非阻塞读取十六进制字符串

private:
    std::string device_;
    speed_t baud_rate_;
    int fd_;
    struct termios oldtio_;

    bool openPort();
    bool setBaudRate(struct termios &newtio);
    bool setPortOptions(struct termios &newtio);
};

#endif