# Dinosaur_Smartcar


## 工程架构
```
.
├── [.cursor] 存放Cursor的回答规则
├── [.vscode] 存放VSCode的配置文件(需要修改opencv路径和编译链路径)
├── [config] 存放配置文件
├── [demo] 包含一些demo文件
├── [include] 头文件
│   ├── [data_config.h] 全局参数定义头文件
│   ├── [head_system.h] 工程调用的系统头文件
│   ├── [json.hpp] JSON库
│   └── [main.h] 主程序头文件
├── [lib] 库文件
│   ├── [control] 控制库
│   │   ├── [encoder.cpp] 编码器库实现文件
│   │   ├── [encoder.h] 编码器库头文件
│   │   ├── [GPIO.cpp] GPIO库实现文件
│   │   ├── [GPIO.h] GPIO库头文件
│   │   ├── [key.cpp] 按键库实现文件
│   │   ├── [key.h] 按键库头文件
│   │   ├── [memory.cpp] 内存监控库实现文件
│   │   ├── [memory.h] 内存监控库头文件
│   │   ├── [pwm_ctrl.cpp] PWM库实现文件
│   │   ├── [pwm_ctrl.h] PWM库头文件 
│   │   ├── [register.cpp] 寄存器库实现文件
│   │   ├── [register.h] 寄存器库头文件 
│   │   ├── [tty.cpp] 串口库实现文件
│   │   ├── [tty.h] 串口库头文件
│   │   ├── [udp.cpp] udp发送库实现文件
│   │   └── [udp.h] udp发送库头文件
│   ├── [logic] 逻辑算法库
│   │   ├── [data_process.cpp] 数据处理实现文件
│   │   ├── [data_process.h] 数据处理头文件
│   │   ├── [image_process.cpp] 图像处理实现文件
│   │   ├── [image_process.h] 图像处理头文件
│   │   ├── [path_across.cpp] 十字赛道操作库
│   │   ├── [path_circle.cpp] 圆环赛道操作库
│   │   ├── [path_side_search.cpp] 赛道边界、路径识别
│   │   ├── [path.h] 路径处理头文件
│   │   ├── [pid.cpp] PID库实现文件
│   │   └── [pid.h] PID库头文件
├── [zf_common] 逐飞功能库
├── [zf_device] 逐飞设备库
├── [src] 源文件
│   ├── [globle.cpp] 全局变量定义文件
│   ├── [main.cpp] 主程序实现文件
│   ├── [program.cpp] 程序管理文件
│   └── [thread.cpp] 线程实现文件
├── [.gitignore] 忽略文件
├── [久久派引脚功能复用表.md] 久久派引脚功能复用表
├── [CMakeLists.txt] CMake编译文件 （需要修改opencv路径）
├── [go] 自动编译脚本
├── [LICENSE] 开源协议
├── [README.md] 工程描述文件
├── [receiver] 视频接收器(需要适配端口号)
├── [smart_car_control] 智能车控制程序
├── [video_receiver.cpp] 视频接收器源文件
└── [vmlinuz] 内核镜像
```
## 视频接收器使用
```bash
./receiver
```

## 脚本使用
### 编译
```bash
./go
```

### 构建makefile(创建或删除了文件时使用)
```bash
./go -b
```

### 清理中间文件
```bash
./go -c
```

### 编译视频接收器
```bash
./go -v
```

## 链接
 第二十届智能车-极速光电龙芯组完赛代码:https://github.com/0Leeeezy0/20th_smart_car_loongson
 ls2k0300_peripheral_library:https://github.com/AirFortressIlikara/ls2k0300_peripheral_library
 逐飞龙芯LS2K0300开源库https://gitee.com/seekfree/LS2K0300_Library