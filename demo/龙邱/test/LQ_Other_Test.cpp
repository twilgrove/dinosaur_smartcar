#include "main.hpp"

// 计算帧率
void CalculateFrameRate()
{
    static bool flags = true;       // 判断第一次进入函数标志位
    static uint8_t fps_count = 0;   // 帧数累计
    static double start_time = 0;   // 开始时间
    static double end_time = 0;     // 结束时间
    if (flags)
    {
        start_time = getTickCount();
        flags = false;
    }
    fps_count++;
    if ((fps_count % 10) == 0)
    {
        end_time = (getTickCount() - start_time) / getTickFrequency();
        printf("fps:%0.3f\n", fps_count / end_time);
        start_time = getTickCount();
        fps_count = 0;
    }
}

// 时间戳打印测试
void GetTimeTest()
{
    void (*printTime)(void);
#if 0
    printTime = GetCurrentMillisecond;
#else
    printTime = printMicTimestamp;
#endif
    while(1)
    {
        printTime();
        sleep(1);
    }
}

// 获取当前微秒值
void GetCurrentMillisecond()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("%s: %lld\n", __func__, (long long)(ts.tv_sec) * 1000000LL + (long long)(ts.tv_nsec) / 1000LL);
}

// 获取当前微秒值
void printMicTimestamp()
{
    struct timeval ts;
    gettimeofday(&ts, NULL);
    printf("%s: %lld\n", __func__, (long long int)ts.tv_sec * 1000000LL + (long long int)ts.tv_usec);
}
