#!/bin/bash

BUILD_DIR="build"
EXECUTABLE="smart_car_control"  # 替换为您的可执行文件名
BUILD_COUNT=128
# 清理功能
clean() {
    echo -e "\033[35m---------------------------------------------------清理生成文件成功----------------------------------------------\033[0m"
    rm -rf "$BUILD_DIR"
    rm -f "$EXECUTABLE"
}

# 编译功能
build() {
    clear
    echo -e "\033[7;32m---------------------------------------------------正在执行Cmake----------------------------------------------\033[0m"
    mkdir -p "$BUILD_DIR" && cd "$BUILD_DIR" && cmake .. 
    echo -e "\033[7;32m---------------------------------------------------正在执行Make----------------------------------------------\033[0m"
    make -j4
    if [ -f "$EXECUTABLE" ]; then
        mv "$EXECUTABLE" ..
        cd ..
        rm -rf "$BUILD_DIR"
        BUILD_COUNT=$((BUILD_COUNT + 1))
        sed -i "s/^BUILD_COUNT=.*/BUILD_COUNT=$BUILD_COUNT/" go
        echo -e "\033[7;33m-------------------------第 $BUILD_COUNT 次编译成功！可执行文件 "$EXECUTABLE" 已生成到项目根目录------------------------\033[0m"
    else
        echo -e "\033[7;31m-------------------------------------------编译失败！未找到可执行文件--------------------------------------------\033[0m" >&2 
        exit 1
    fi
}

# 参数解析
case "$1" in
    -c)
        clean
        ;;
    *)
        build
        ;;
esac