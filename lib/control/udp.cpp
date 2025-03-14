
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
    if (!cv::imencode(".jpg", frame, buffer, {cv::IMWRITE_JPEG_QUALITY, 40})) // 将图像编码为JPEG格式
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
void udp::sendFrame_pro(const cv::Mat &frame)
{
    if (!isEnabled)
        return;

    // JPEG编码
    std::vector<uchar> buffer;
    if (!cv::imencode(".jpg", frame, buffer, {cv::IMWRITE_JPEG_QUALITY, 40}))
    {
        std::cerr << "Encode failed" << std::endl;
        return;
    }

    const int HEADER_SIZE = 8; // 帧头大小:4(frameID)+2(frag#)+2(total)
    const int MAX_CHUNK = PACK_SIZE - HEADER_SIZE;

    // 计算分片数量
    const int total_size = buffer.size();
    const int total_frags = (total_size + MAX_CHUNK - 1) / MAX_CHUNK;

    // 构造并发送每个分片
    char packet[PACK_SIZE];
    for (int i = 0; i < total_size; i += MAX_CHUNK)
    {
        const int frag_id = i / MAX_CHUNK;
        const int chunk_size = std::min(MAX_CHUNK, total_size - i);

        // 填充协议头（网络字节序）
        uint32_t net_frame = htonl(frameID);
        uint16_t net_frag = htons(frag_id);
        uint16_t net_total = htons(total_frags);

        memcpy(packet, &net_frame, 4);
        memcpy(packet + 4, &net_frag, 2);
        memcpy(packet + 6, &net_total, 2);

        // 填充数据
        memcpy(packet + HEADER_SIZE, buffer.data() + i, chunk_size);

        // 发送
        if (sendto(sock, packet, HEADER_SIZE + chunk_size, 0,
                   (sockaddr *)&destAddr, sizeof(destAddr)) == -1)
        {
            std::cerr << "Send error: " << strerror(errno) << std::endl;
        }
    }

    frameID++; // 递增帧号
}