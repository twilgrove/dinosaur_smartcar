#ifndef UDP_H
#define UDP_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <cerrno>
#include <cstring>
#define PACK_SIZE 1024 // UDP 包大小

class udp
{
public:
    udp(const std::string &dest_ip, int dest_port, bool enable_sending); // 构造函数
    ~udp();
    void sendFrame(const cv::Mat &frame);     // 发送帧
    void sendFrame_pro(const cv::Mat &frame); // 发送帧(加帧头校验)
    void Enable(bool enable);                 // 设置是否启用

private:
    int sock;             // 套接字
    sockaddr_in destAddr; // 目标地址
    std::string destIP;   // 目标IP
    int destPort;         // 目标端口
    bool isEnabled;       // 是否启用
    uint32_t frameID = 0; // 帧序列号
};
#endif