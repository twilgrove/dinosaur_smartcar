cmake_minimum_required(VERSION 3.10)

# 项目名称
project(smart_car_control VERSION 1.0.0 LANGUAGES C CXX)

# 路径
set(OPENCV_PATH "/home/cys/Project_Date/LS2K0300_workspace/opencv") # 设置opencv路径

# 交叉编译设置
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR loongarch64)

# 编译器设置
set(CMAKE_C_COMPILER loongarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER loongarch64-linux-gnu-g++)

# 语言标准设置
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件收集（推荐显式列出，这里保持GLOB但增加CONFIGURE_DEPENDS）
file(GLOB_RECURSE SOURCES
    "src/*.c"
    "src/*.cpp"
)

# 输出信息
string(ASCII 27 Esc)

foreach(source IN LISTS SOURCES)
    get_filename_component(absolute_path "${source}" ABSOLUTE)
    file(RELATIVE_PATH relative_path "${CMAKE_SOURCE_DIR}" "${absolute_path}")
    message(STATUS "${Esc}[36m源文件: ${Esc}[35m./${relative_path}${Esc}[0m")
endforeach()

# 可执行文件目标
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${OPENCV_PATH}/install/include
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    pthread
    ${OPENCV_PATH}/install/lib/libopencv_core.so
    ${OPENCV_PATH}/install/lib/libopencv_imgproc.so
    ${OPENCV_PATH}/install/lib/libopencv_imgcodecs.so
    ${OPENCV_PATH}/install/lib/libopencv_highgui.so
    ${OPENCV_PATH}/install/lib/libopencv_videoio.so
)

# 编译选项
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)
