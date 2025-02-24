// #include <opencv2/opencv.hpp>
// #include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "main.h"

#define SERVER_PORT 12345
#define SERVER_IP "192.168.1.100" // 电脑的 IP 地址

int main()
{
    pwm_init(0, 0, 0, 0);
    // // 创建 UDP 套接字
    // int sock = socket(AF_INET, SOCK_DGRAM, 0);
    // if (sock < 0)
    // {
    //     std::cerr << "无法创建套接字!" << std::endl;
    //     return -1;
    // }

    // // 设置目标服务器地址（电脑的 IP 和端口）
    // sockaddr_in server_addr;
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_port = htons(SERVER_PORT);
    // server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // // 创建 VideoCapture 对象，打开摄像头
    // cv::VideoCapture cap(0); // 0 表示默认摄像头
    // if (!cap.isOpened())
    // {
    //     std::cerr << "无法打开摄像头!" << std::endl;
    //     return -1;
    // }

    // cv::Mat frame;
    // while (true)
    // {
    //     cap >> frame; // 捕获一帧图像
    //     if (frame.empty())
    //     {
    //         std::cerr << "无法捕获图像!" << std::endl;
    //         break;
    //     }

    //     // 将图像编码为 JPEG 格式
    //     std::vector<uchar> buffer;
    //     cv::imencode(".jpg", frame, buffer);
    //     sendto(sock, buffer.data(), buffer.size(), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    // }

    // cap.release();
    // close(sock); // 关闭套接字
    return 0;
}
