#ifndef TTY_H
#define TTY_H

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <stdarg.h>

class SerialPort
{
public:
    SerialPort(const std::string &device, speed_t baud_rate);
    ~SerialPort();

    void printf(const char *fmt, ...); // 写入数据
    bool readData(std::string &data);  // 非阻塞读取数据

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