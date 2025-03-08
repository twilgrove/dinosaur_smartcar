#include "tty.h"

/**
 * 功能: 构造函数
 * 参数: 无
 * 返回: 无
 */
SerialPort::SerialPort(const std::string &device, speed_t baud_rate)
    : device_(device), baud_rate_(baud_rate), fd_(-1)
{
    if (openPort())
        std::cout << "\33[34mSerialPort:\33[0m" << device_ << " init success" << std::endl;
}

/**
 * 功能: 析构函数
 * 参数: 无
 * 返回: 无
 */
SerialPort::~SerialPort()
{
    if (fd_ != -1)
    {
        // 恢复串口的旧设置
        tcsetattr(fd_, TCSANOW, &oldtio_);
        close(fd_);
        std::cout << "\33[34mSerialPort:\33[0m" << device_ << " close success" << std::endl;
    }
}

// 修改函数签名，接收 termios 引用作为参数
bool SerialPort::setBaudRate(struct termios &newtio) // 移除内部 tcgetattr
{
    // 设置波特率（直接操作传入的结构体）
    if (cfsetispeed(&newtio, baud_rate_) != 0 || cfsetospeed(&newtio, baud_rate_) != 0)
    {
        std::cerr << "\33[31mSerialPort:\33[0m Failed to set baud rate" << std::endl;
        return false;
    }
    return true;
}

bool SerialPort::setPortOptions(struct termios &newtio) // 移除内部 tcgetattr
{
    // 设置数据位、停止位、校验位等（直接操作传入的结构体）
    newtio.c_cflag &= ~PARENB;        // 无奇偶校验
    newtio.c_cflag &= ~CSTOPB;        // 1 停止位
    newtio.c_cflag &= ~CSIZE;         // 清除数据位设置
    newtio.c_cflag |= CS8;            // 8 数据位
    newtio.c_cflag &= ~CRTSCTS;       // 无硬件流控
    newtio.c_cflag |= CREAD | CLOCAL; // 启用接收，忽略控制线

    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 原始输入模式
    newtio.c_oflag &= ~OPOST;                          // 原始输出模式
    newtio.c_iflag &= ~(IXON | IXOFF | IXANY);         // 禁用软件流控

    newtio.c_cc[VMIN] = 0;  // 最小读取字符数
    newtio.c_cc[VTIME] = 5; // 读取超时 (5*0.1s)

    return true;
}

bool SerialPort::openPort()
{
    fd_ = open(device_.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd_ == -1)
    {
        std::cerr << "\33[31mSerialPort:\33[0m Failed to open serial port" << std::endl;
        return false;
    }

    // 获取并备份原配置
    if (tcgetattr(fd_, &oldtio_) != 0)
    {
        std::cerr << "\33[31mSerialPort:\33[0m Failed to get serial port attributes" << std::endl;
        return false;
    }

    struct termios newtio = oldtio_; // 基于原配置修改

    // 传递 newtio 的引用进行设置
    if (!setBaudRate(newtio))
        return false;
    if (!setPortOptions(newtio))
        return false;

    // 应用新配置
    if (tcsetattr(fd_, TCSANOW, &newtio) != 0)
    {
        std::cerr << "\33[31mSerialPort:\33[0m Failed to set serial port attributes" << std::endl;
        return false;
    }

    fcntl(fd_, F_SETFL, 0);
    return true;
}

/**
 * 功能: 写入数据
 * 参数: fmt: 格式化字符串
 * 返回: 是否成功
 */
void SerialPort::printf(const char *fmt, ...)
{
    if (fd_ == -1)
        return;

    // 使用可变参数格式化字符串
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    ssize_t bytes_written = write(fd_, buffer, strlen(buffer));
    if (bytes_written == -1)
    {
        std::cerr << "\33[31mSerialPort:\33[0m Failed to write data to serial port" << std::endl;
        return;
    }
}

/**
 * 功能: 读取数据
 * 参数: data: 读取到的数据
 * 返回: 是否成功
 */
bool SerialPort::readString(std::string &data)
{
    if (fd_ == -1)
        return false;

    char buffer[256];
    ssize_t bytes_read = read(fd_, buffer, sizeof(buffer) - 1);

    if (bytes_read == -1)
    {
        // 处理错误（非阻塞模式下可能为 EAGAIN）
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            std::cerr << "\33[31mSerialPort:\33[0m Read error: " << strerror(errno) << std::endl;
        return false;
    }
    else if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';
        data = std::string(buffer);
        return true;
    }

    return false;
}
/**
 * 功能: 读取数据并转换为十六进制字符串 (如 "1A2B3C")
 * 参数: data - 输出的十六进制字符串
 * 返回: 是否成功读取到数据
 */
bool SerialPort::readHexString(std::string &hex_data)
{
    if (fd_ == -1)
        return false;

    uint8_t buffer[256];
    ssize_t bytes_read = read(fd_, buffer, sizeof(buffer));

    if (bytes_read == -1)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            std::cerr << "\33[31mSerialPort:\33[0m Read error: " << strerror(errno) << std::endl;
        return false;
    }
    else if (bytes_read > 0)
    {
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0');
        for (int i = 0; i < bytes_read; ++i)
        {
            // if (i > 0)
            //     ss << " "; // 用空格分隔（可改为":"等）
            ss << std::setw(2) << static_cast<int>(buffer[i]);
        }
        hex_data = ss.str();
        return true;
    }
    return false;
}

/**
 * 功能: 读取数据
 * 参数: data: 读取到的数据
 * 返回: 是否成功
 */
bool SerialPort::readData(uint8_t *data, size_t length)
{
    if (fd_ == -1)
        return false;

    ssize_t bytes_read = read(fd_, data, length);

    if (bytes_read == -1)
    {
        // 处理错误（非阻塞模式下可能为 EAGAIN）
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            std::cerr << "\33[31mSerialPort:\33[0m Read error: " << strerror(errno) << std::endl;
        return false;
    }
    else if (bytes_read > 0)
    {
        return true;
    }

    return false;
}
