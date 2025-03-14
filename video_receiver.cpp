#include <iostream>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <chrono>

#define PACK_SIZE 1024            // 必须与发送端一致
#define PORT 8888                 // 监听端口
#define MAX_CHUNK (PACK_SIZE - 8) // 数据分片大小（协议头占8字节）
#define CLEANUP_INTERVAL 2000     // 清理间隔(ms)
#define FRAME_TIMEOUT 3000        // 帧超时时间(ms)

// 帧重组上下文
struct FrameContext
{
    std::vector<bool> received;                        // 分片到达状态
    std::vector<uchar> data;                           // 数据存储
    std::chrono::steady_clock::time_point create_time; // 创建时间
    int total_frags;                                   // 总分片数
    int received_count = 0;                            // 已收到分片数（优化遍历）
    int valid_size = 0;                                // 实际有效数据总长度
};

std::unordered_map<uint32_t, FrameContext> frame_map; // 正在组装的帧
uint32_t last_shown_frame = 0;                        // 最后显示的帧号（处理乱序）

int main()
{
    // 创建socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Socket failed: " << strerror(errno) << std::endl;
        return -1;
    }

    // 绑定端口
    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Listening on port " << PORT << std::endl;
    cv::namedWindow("Video Stream", cv::WINDOW_NORMAL);
    cv::resizeWindow("Video Stream", 640, 480);

    auto last_clean = std::chrono::steady_clock::now(); // 上次清理时间
    char packet[PACK_SIZE];                             // 数据包

    while (true)
    {
        // 接收UDP数据包
        sockaddr_in cliaddr{};
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, packet, sizeof(packet), 0,
                         (sockaddr *)&cliaddr, &len);

        if (n < 8)
            continue; // 忽略无效包（协议头至少8字节）

        // 解析协议头（网络字节序转主机字节序）
        uint32_t frame_id = ntohl(*reinterpret_cast<uint32_t *>(packet));
        uint16_t frag_id = ntohs(*reinterpret_cast<uint16_t *>(packet + 4));
        uint16_t total_frags = ntohs(*reinterpret_cast<uint16_t *>(packet + 6));

        // 有效性检查
        if (total_frags == 0 || frag_id >= total_frags || (n - 8) > MAX_CHUNK)
        {
            std::cerr << "Invalid packet: frame=" << frame_id
                      << " frag=" << frag_id << "/" << total_frags << std::endl;
            continue;
        }

        // 获取或创建帧上下文
        auto &ctx = frame_map[frame_id];
        // 修正后的初始化代码
        if (ctx.received.empty())
        {
            ctx.total_frags = total_frags;
            ctx.received.resize(total_frags, false);

            // 安全初始化：分配最大可能空间 (total_frags * MAX_CHUNK)
            ctx.data.resize(total_frags * MAX_CHUNK);

            ctx.create_time = std::chrono::steady_clock::now();
        }

        // 存储分片数据（允许重复接收）
        if (!ctx.received[frag_id])
        {
            // 计算数据拷贝位置
            int offset = frag_id * MAX_CHUNK;
            int copy_size = n - 8; // 减去协议头

            // 防止最后一个分片越界
            if (offset + copy_size > ctx.data.size())
            {
                std::cerr << "Fragment overflow: frame=" << frame_id
                          << " frag=" << frag_id << std::endl;
                continue;
            }
            // 动态更新有效数据终点
            ctx.valid_size = std::max(ctx.valid_size, offset + copy_size);

            memcpy(ctx.data.data() + offset, packet + 8, copy_size);
            ctx.received[frag_id] = true;
            ctx.received_count++;
        }

        // 检查帧是否完整
        if (ctx.received_count == ctx.total_frags)
        {
            // 解码图像（仅显示比上一帧新的）
            if (frame_id > last_shown_frame)
            {
                std::vector<uchar> valid_data(ctx.data.begin(),
                                              ctx.data.begin() + ctx.valid_size);
                cv::Mat frame = cv::imdecode(ctx.data, cv::IMREAD_COLOR);
                if (!frame.empty())
                {
                    cv::imshow("Video Stream", frame);
                    last_shown_frame = frame_id;
                    cv::waitKey(1);
                }
            }
            frame_map.erase(frame_id); // 移除已完成的帧
        }

        // 定期清理超时帧
        auto now = std::chrono::steady_clock::now();
        if (now - last_clean > std::chrono::milliseconds(CLEANUP_INTERVAL))
        {
            for (auto it = frame_map.begin(); it != frame_map.end();)
            {
                if (now - it->second.create_time >
                    std::chrono::milliseconds(FRAME_TIMEOUT))
                {
                    std::cout << "Cleanup frame: " << it->first << std::endl;
                    it = frame_map.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            last_clean = now;
        }
    }

    close(sockfd);
    cv::destroyAllWindows();
    return 0;
}