#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <iostream>
#include <fstream>
#include <string>

class MemoryMonitor
{
public:
    MemoryMonitor();  // 构造函数：打开文件
    ~MemoryMonitor(); // 析构函数：关闭文件

    double getVmSize(); // 获取虚拟内存 (MB)
    double getVmRSS();  // 获取物理内存 (MB)
    double getVmData(); // 获取数据段 (MB)
    double getVmSwap(); // 获取交换空间 (MB)

private:
    std::ifstream file; // 文件流

    double extractValue(const std::string &keyword); // 解析 `/proc/self/status`
};

#endif // MEMORY_MONITOR_H
