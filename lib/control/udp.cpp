
#include "udp.h"
#include <cerrno>
#include <cstring>

udp::udp(const std::string &dest_ip, int dest_port, bool enable_sending)
    : destIP(dest_ip), destPort(dest_port), isEnabled(enable_sending)
{
    /*创建套接字*/
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        std::cerr << "create socket failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    /*设置目标地址*/
    destAddr.sin_family = AF_INET;                          // 协议族
    destAddr.sin_port = htons(destPort);                    // 端口号
    inet_pton(AF_INET, destIP.c_str(), &destAddr.sin_addr); // 将IP地址转换为网络字节序

    std::cout << "udp init complete" << std::endl;
}

udp::~udp()
{
    close(sock);
}

void udp::Enable(bool enable)
{
    isEnabled = enable;
}

void udp::sendFrame(const cv::Mat &frame)
{
    if (!isEnabled)
        return;

    std::vector<uchar> buffer;
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 100}; // 设置JPEG编码质量
    if (!cv::imencode(".jpg", frame, buffer, params))          // 将图像编码为JPEG格式
    {
        std::cerr << "JPEG encode failed!" << std::endl;
        return;
    }

    int dataSize = buffer.size();
    char *data = reinterpret_cast<char *>(buffer.data());

    int sentBytes = sendto(sock, &dataSize, sizeof(dataSize), 0, (sockaddr *)&destAddr, sizeof(destAddr));
    if (sentBytes == -1)
    {
        std::cerr << "send data size failed: " << strerror(errno) << std::endl;
        return;
    }

    for (int i = 0; i < dataSize; i += PACK_SIZE)
    {
        int chunkSize = std::min(PACK_SIZE, dataSize - i);
        sentBytes = sendto(sock, data + i, chunkSize, 0, (sockaddr *)&destAddr, sizeof(destAddr));
        if (sentBytes == -1)
        {
            std::cerr << "send data failed: " << strerror(errno) << std::endl;
            return;
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
