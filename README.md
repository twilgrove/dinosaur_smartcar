# 二仙桥小恐龙的智能车

## 文件结构
- **`.cursor`**：存放Cursor的回答规则
- **`include`**：包含一些常用的宏定义, 头文件(.h)
- **`libs`**：包含一些常用的通用库文件(.c.h)(封装库、算法库...)
- **`src`**：包含核心代码文件(.c)
- **`vmlinuz`**：内核镜像（内核源码[https://github.com/AirFortressIlikara/LS2K0300-linux-4.19](https://github.com/AirFortressIlikara/LS2K0300-linux-4.19)）

## 编译
```bash
make
```

## 清理
```bash
make clean
```