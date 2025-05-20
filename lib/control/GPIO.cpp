/*
 * @Author: ilikara 3435193369@qq.com
 * @Date: 2024-10-10 15:02:10
 * @LastEditors: Ilikara 3435193369@qq.com
 * @LastEditTime: 2025-02-14 09:54:20
 * @FilePath: /ls2k0300_peripheral_library/src/GPIO.cpp
 * @Description: GPIO类
 *
 * Copyright (c) 2024 by ilikara 3435193369@qq.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include "GPIO.h"

/**
 * 功能: 构造函数1
 * 参数: gpioNum - GPIO编号
 *       direction - GPIO方向
 *       value - GPIO输出值
 * 返回: 无
 */
GPIO::GPIO(int gpioNum, std::string direction, bool value) : gpioNum(gpioNum), fd(-1)
{
    gpioPath = "/sys/class/gpio/gpio" + std::to_string(gpioNum);

    // 导出 GPIO
    if (!writeToFile("/sys/class/gpio/export", std::to_string(gpioNum)))
    {
        throw std::runtime_error("Failed to export GPIO " + std::to_string(gpioNum));
    }

    // 打开 value 文件，读写方式
    fd = open((gpioPath + "/value").c_str(), O_RDWR);
    if (fd == -1)
    {
        throw std::runtime_error("Failed to open GPIO value file: " + std::string(strerror(errno)));
    }

    setDirection(direction);
    setValue(value);

    std::cout << "\33[34mGPIO:\33[0m" << gpioNum << " init success" << std::endl;
}

/**
 * 功能: 构造函数2
 * 参数: gpioNum - GPIO编号
 *       direction - GPIO方向
 * 返回: 无
 */
GPIO::GPIO(int gpioNum, std::string direction) : gpioNum(gpioNum), fd(-1)
{
    gpioPath = "/sys/class/gpio/gpio" + std::to_string(gpioNum);

    // 导出 GPIO
    if (!writeToFile("/sys/class/gpio/export", std::to_string(gpioNum)))
    {
        throw std::runtime_error("Failed to export GPIO " + std::to_string(gpioNum));
    }

    // 打开 value 文件，读写方式
    fd = open((gpioPath + "/value").c_str(), O_RDWR);
    if (fd == -1)
    {
        throw std::runtime_error("Failed to open GPIO value file: " + std::string(strerror(errno)));
    }

    setDirection(direction);

    std::cout << "\33[34mGPIO:\33[0m" << gpioNum << " init success" << std::endl;
}

/**
 * 功能: 析构函数
 * 参数: 无
 * 返回: 无
 */
GPIO::~GPIO(void)
{
    setDirection("in");
    /* 关闭GPIO */
    if (writeToFile("/sys/class/gpio/unexport", std::to_string(gpioNum)))
    {
        std::cout << "\33[34mGPIO:\33[0m" << gpioNum << " close success" << std::endl;
    }

    close(fd);
}

/**
 * 功能: 设置GPIO方向，out为输出，in为输入
 * 参数: direction - GPIO方向
 * 返回: 无
 */
bool GPIO::setDirection(const std::string &direction)
{
    return writeToFile(gpioPath + "/direction", direction);
}

/**
 * 功能: 设置GPIO边缘触发
 * 参数: edge - GPIO边缘触发
 * 返回: 无
 */
bool GPIO::setEdge(const std::string &edge)
{
    return writeToFile(gpioPath + "/edge", edge);
}

/**
 * 功能: 设置 GPIO 输出值
 * 参数: value - GPIO输出值
 * 返回: 无
 */
bool GPIO::setValue(bool value)
{
    // 使用文件描述符写入 GPIO 值 ('1' 或 '0')
    const char *val_str = value ? "1" : "0";
    if (write(fd, val_str, 1) != 1)
    {
        throw std::runtime_error("Failed to write GPIO value: " + std::string(strerror(errno)));
    }
    return true;
}

/**
 * 功能: 读取 GPIO 输入值
 * 参数: 无
 * 返回: GPIO 输入值
 */
bool GPIO::readValue(void)
{
    char value;
    lseek(fd, 0, SEEK_SET); // 重置文件偏移量
    if (read(fd, &value, 1) != 1)
    {
        throw std::runtime_error("Failed to read GPIO value: " + std::string(strerror(errno)));
    }
    return value == '1'; // 如果读取的值为 '1'，则返回 true，否则返回 false
}

/**
 * 功能: 获取 GPIO 文件描述符
 * 参数: 无
 * 返回: GPIO 文件描述符
 */
int GPIO::getFileDescriptor(void) const
{
    return fd;
}

bool GPIO::writeToFile(const std::string &path, const std::string &value)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file " + path + ": " + std::string(strerror(errno)));
    }
    file << value;
    return file.good();
}
