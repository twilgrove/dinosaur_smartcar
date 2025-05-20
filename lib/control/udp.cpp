#include <opencv2/opencv.hpp>
#include "udp.h"
#include <cerrno>
#include <cstring>


bool UdpSender::init(const std::string& ip, uint16_t port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);//创建一个 UDP 套接字

    if (sockfd < 0) {
        std::cerr << "Socket 创建失败" << std::endl;
        return false;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;//设置地址族为 IPv4
    dest_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &dest_addr.sin_addr) <= 0) {
        std::cerr << "无效目标IP地址" << std::endl;
        close(sockfd);
        return false;
    }//将传入的目标 IP 地址字符串转换为二进制形式

    return true;
}

void UdpSender::closeSender() {
    if (sockfd >= 0)
        close(sockfd);
}

UdpSender::~UdpSender() {
    closeSender(); // 程序退出或对象销毁时自动释放资源
}

bool UdpSender::sendImage(const cv::Mat& frame, int quality) {
    std::vector<uchar> img_buf;//存储编码后的图像数据
    std::vector<int> encode_params = {cv::IMWRITE_JPEG_QUALITY, quality};//设置图像编码的参数

    if (!cv::imencode(".jpg", frame, img_buf, encode_params)) {
        std::cerr << "图像编码失败" << std::endl;
        return false;
    }

    size_t total_size = img_buf.size();
    const char* data_ptr = reinterpret_cast<const char*>(img_buf.data());

    uint32_t pkt_header[2] = {
        htonl(0xABCD1234),
        htonl(static_cast<uint32_t>(total_size))
    };

    sendto(sockfd, pkt_header, sizeof(pkt_header), 0,
           (struct sockaddr*)&dest_addr, sizeof(dest_addr));

    size_t sent = 0;
    while (sent < total_size) {
        size_t chunk_size = std::min(static_cast<size_t>(1024), total_size - sent);

        ssize_t bytes_sent = sendto(sockfd, data_ptr + sent, chunk_size, 0,
                                    (struct sockaddr*)&dest_addr, sizeof(dest_addr));//如果发送成功，bytes_sent 会返回实际发送的字节数
        if (bytes_sent < 0) {
            std::cerr << "数据发送失败" << std::endl;
            return false;
        }
        sent += bytes_sent;
    }

    return true;
}