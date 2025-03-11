
#include "udp.h"
#include <cerrno>
#include <cstring>

VideoSender::VideoSender(const std::string &dest_ip, int dest_port, bool enable_sending)
    : destIP(dest_ip), destPort(dest_port), isEnabled(enable_sending)
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        std::cerr << "create socket failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destPort);
    inet_pton(AF_INET, destIP.c_str(), &destAddr.sin_addr);
    std::cout << "udp init complete" << std::endl;
}

VideoSender::~VideoSender()
{
    close(sock);
}

void VideoSender::setEnabled(bool enable)
{
    isEnabled = enable;
}

void VideoSender::sendFrame(const cv::Mat &frame)
{
    if (!isEnabled)
        return;

    std::vector<uchar> buffer;
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 20};
    if (!cv::imencode(".jpg", frame, buffer, params))
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

    for (int i = 0; i < dataSize; i += CHUNK_SIZE)
    {
        int chunkSize = std::min(CHUNK_SIZE, dataSize - i);
        sentBytes = sendto(sock, data + i, chunkSize, 0, (sockaddr *)&destAddr, sizeof(destAddr));
        if (sentBytes == -1)
        {
            std::cerr << "send data failed: " << strerror(errno) << std::endl;
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
