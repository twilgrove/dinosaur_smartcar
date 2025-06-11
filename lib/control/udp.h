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

class UdpSender
{
public:
    bool init(const std::string &ip, uint16_t port);
    void closeSender();
    bool sendImage(const cv::Mat &frame, int quality = 100);
    ~UdpSender();

private:
    int sockfd;
    sockaddr_in dest_addr{};
};

#endif