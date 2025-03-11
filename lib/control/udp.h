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
#define CHUNK_SIZE 1024 // UDP 最大安全包大小

class VideoSender
{
public:
    VideoSender(const std::string &dest_ip, int dest_port, bool enable_sending);
    ~VideoSender();
    void sendFrame(const cv::Mat &frame);
    void setEnabled(bool enable);

private:
    int sock;
    sockaddr_in destAddr;
    std::string destIP;
    int destPort;
    bool isEnabled;
};
/**
#endif
#include "udp.hpp"
#include <opencv2/opencv.hpp>

int main() {
    std::string ip = "192.168.123.119";
    int port = 8888;
    bool enableSending = true;

    VideoSender sender(ip, port, enableSending);

    cv::VideoCapture cap("/home/an/Desktop/hello/cao.mp4");
    if (!cap.isOpened()) {
        std::cerr << "无法打开视频文件！" << std::endl;
        return -1;
    } else {
        std::cout << "视频已打开，开始发送..." << std::endl;
    }

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        sender.sendFrame(frame);
        std::cout << "发送一帧数据" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    cap.release();
    return 0;
}
**/

#endif