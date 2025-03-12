#include <iostream>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <cstring>

#define PACK_SIZE 1024
#define PORT 8888 // 需要与发送端的目标端口一致

int main()
{
    // 创建UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        return -1;
    }

    // 绑定地址和端口
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Receiver started on port " << PORT << std::endl;

    // cv::namedWindow("Video Stream", cv::WINDOW_AUTOSIZE); // 创建窗口
    cv::namedWindow("Video Stream", cv::WINDOW_NORMAL); // 允许调整窗口尺寸
    cv::resizeWindow("Video Stream", 640, 480);         // 设置初始窗口大小

    while (true)
    {
        // 接收数据大小
        int total_size;
        sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);

        int n = recvfrom(sockfd, &total_size, sizeof(total_size), 0,
                         (sockaddr *)&cliaddr, &len);
        if (n != sizeof(total_size))
        {
            std::cerr << "Failed to receive size header" << std::endl;
            continue;
        }

        // 转换网络字节序（如果发送端有转换的话）
        // total_size = ntohl(total_size);

        // 验证数据大小
        if (total_size <= 0 || total_size > 1024 * 1024)
        { // 限制最大1MB
            std::cerr << "Invalid data size: " << total_size << std::endl;
            continue;
        }

        // 准备接收数据
        std::vector<uchar> buffer(total_size);
        int received = 0;

        while (received < total_size)
        {
            uchar chunk[PACK_SIZE];
            int remaining = total_size - received;
            int chunk_size = std::min(remaining, PACK_SIZE);

            n = recvfrom(sockfd, chunk, chunk_size, 0,
                         (sockaddr *)&cliaddr, &len);
            if (n <= 0)
            {
                std::cerr << "Receive error: " << strerror(errno) << std::endl;
                break;
            }

            memcpy(buffer.data() + received, chunk, n);
            received += n;
        }

        // 解码并显示图像
        if (received == total_size)
        {
            cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);
            if (!frame.empty())
            {
                cv::imshow("Video Stream", frame);
                if (cv::waitKey(1) == 27)
                    exit(0); // ESC退出
            }
        }
        else
        {
            std::cerr << "Incomplete frame received (" << received
                      << "/" << total_size << " bytes)" << std::endl;
        }
    }

    close(sockfd);           // 关闭socket
    cv::destroyAllWindows(); // 销毁窗口
    return 0;
}