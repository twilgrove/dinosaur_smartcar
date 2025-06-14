#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#define OUTOUT_ENABLE 0
#define PORT 8080
#define MAX_PACKET_SIZE 1024
#define HEADER_SIZE 8
#define MAGIC_NUMBER 0xABCD1234

int main()
{
    // 创建 UDP 套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Socket 创建失败" << std::endl;
        return -1;
    }

    // 配置本地地址
    sockaddr_in recv_addr{};
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(PORT);
    recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0)
    {
        std::cerr << "Socket 绑定失败" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "接收器已启动，等待图像..." << std::endl;

#if OUTOUT_ENABLE
    cv::VideoWriter writer;
    bool writer_initialized = false;
#endif
    cv::namedWindow("Received Image", cv::WINDOW_NORMAL);
    while (true)
    {
        // 接收头部
        char header_buf[HEADER_SIZE];
        sockaddr_in sender_addr{};
        socklen_t sender_len = sizeof(sender_addr);

        int header_received = recvfrom(sockfd, header_buf, HEADER_SIZE, 0,
                                       (struct sockaddr *)&sender_addr, &sender_len);
        if (header_received != HEADER_SIZE)
        {
            std::cerr << "头部接收失败" << std::endl;
            continue;
        }

        uint32_t *header = reinterpret_cast<uint32_t *>(header_buf);
        uint32_t magic = ntohl(header[0]);
        uint32_t total_size = ntohl(header[1]);

        if (magic != MAGIC_NUMBER)
        {
            std::cerr << "无效头部标识" << std::endl;
            continue;
        }

        std::vector<uchar> img_buf(total_size);
        size_t received_total = 0;

        while (received_total < total_size)
        {
            int len = recvfrom(sockfd, (char *)img_buf.data() + received_total,
                               std::min(MAX_PACKET_SIZE, (int)(total_size - received_total)),
                               0, (struct sockaddr *)&sender_addr, &sender_len);
            if (len <= 0)
            {
                std::cerr << "图像数据接收中断" << std::endl;
                break;
            }
            received_total += len;
        }

        // 解码图像并显示
        cv::Mat img = cv::imdecode(img_buf, cv::IMREAD_COLOR);
        cv::resizeWindow("Received Image", 935, 400);
        if (!img.empty())
        {
            cv::imshow("Received Image", img);
            if (cv::waitKey(1) == 'q')
            {
                std::cout << "按下 'q' 键，退出程序" << std::endl;
                break;
            }

#if OUTOUT_ENABLE
            if (!writer_initialized)
            {
                writer.open("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30,
                            img.size());
                writer_initialized = writer.isOpened();
            }

            if (writer_initialized)
            {
                writer.write(img);
            }
#endif
        }
        else
        {
            std::cerr << "图像解码失败" << std::endl;
        }
    }

    close(sockfd);
    return 0;
}
