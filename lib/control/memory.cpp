#include "memory.h"

MemoryMonitor::MemoryMonitor()
{
    file.open("/proc/self/status");
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open /proc/self/status" << std::endl;
    }
}

MemoryMonitor::~MemoryMonitor()
{
    if (file.is_open())
    {
        file.close(); // 关闭文件
    }
}

// 获取 VmSize (虚拟内存)
double MemoryMonitor::getVmSize()
{
    return extractValue("VmSize:");
}

// 获取 VmRSS (物理内存)
double MemoryMonitor::getVmRSS()
{
    return extractValue("VmRSS:");
}

// 获取 VmData (数据段)
double MemoryMonitor::getVmData()
{
    return extractValue("VmData:");
}

// 获取 VmSwap (交换空间)
double MemoryMonitor::getVmSwap()
{
    return extractValue("VmSwap:");
}

// 提取 `/proc/self/status` 指定字段的值
double MemoryMonitor::extractValue(const std::string &keyword)
{
    if (!file.is_open())
        return 0.0;

    std::string line;
    file.clear();                 // 清除 EOF 标志，重新读取
    file.seekg(0, std::ios::beg); // 重新定位到文件开头

    while (std::getline(file, line))
    {
        if (line.find(keyword) != std::string::npos)
        {
            size_t pos = line.find(":");
            if (pos != std::string::npos)
            {
                double value_kB = std::stod(line.substr(pos + 1));
                return value_kB / 1024.0; // 转换为 MB
            }
        }
    }

    return 0.0;
}
