# Dinosaur_Smartcar


## 工程架构
```
.
├── [.vscode] 存放VSCode的配置文件(需要修改opencv路径和编译链路径)
├── [include] 头文件
├── [lib] 库文件
│   ├── [control] 控制库
│   ├── [logic] 逻辑算法库
│   ├── [zf_common] 逐飞功能库
│   ├── [zf_device] 逐飞设备库
├── [src] 源文件
├── [.gitignore] 忽略文件
├── [久久派引脚功能复用表.md] 久久派引脚功能复用表
├── [CMakeLists.txt] CMake编译文件 （需要修改opencv路径）
├── [go] 自动编译脚本
├── [LICENSE] 开源协议
├── [README.md] 工程描述文件
├── [rec] 图传程序
├── [recv_cam.cpp] 图传程序源码
├── [smart_car_control] 智能车控制程序
└── [vmlinuz] 内核镜像
```
## 视频接收器使用
```bash
./rec
```

## 脚本使用
### 编译智能车控制程序
```bash
./go
```

### 构建智能车控制程序的makefile(创建或删除了文件时使用)
```bash
./go -b
```

### 清理智能车控制程序中间文件
```bash
./go -c
```
### 编译图传程序(Linux)
```bash
./go -r
```

## 链接
 第二十届智能车-极速光电龙芯组完赛代码:https://github.com/0Leeeezy0/20th_smart_car_loongson
 ls2k0300_peripheral_library:https://github.com/AirFortressIlikara/ls2k0300_peripheral_library
 逐飞龙芯LS2K0300开源库https://gitee.com/seekfree/LS2K0300_Library