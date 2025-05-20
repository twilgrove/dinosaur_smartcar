#include "main.hpp"

// sleep()函数测试 -- 以秒为单位延时
void sleepTest()
{
    while (1)
    {
        printf("sleep function delay 1s\n");
        sleep(1);
    }
}

// usleep()函数测试 -- 以微秒为单位延时
void usleepTest()
{
    while (1)
    {
        printf("usleep function delay 0.5s\n");
        usleep(500000); // 延时 0.5 秒
    }
}

// nanosleep()函数测试 -- 以纳秒为单位延时
void nanosleepTest()
{
    struct timespec req;    // 想要延时的时间放这里
    struct timespec rem;    // 函数输出参，函数若被信号打断，剩余时间返回到该参数
    req.tv_sec = 0;             // 秒数
    req.tv_nsec = 500000000;    // 500000000 纳秒 = 0.5 秒
    while(1)
    {
        printf("nanosleep function delay 0.5s\n");
        nanosleep(&req, &rem);
    }
}

// clock_nanosleep()函数测试 -- 以纳秒为单位延时
void clock_nanosleepTest()
{
    struct timespec req;    // 想要延时的时间放这里
    struct timespec rem;    // 函数输出参，函数若被信号打断，剩余时间返回到该参数
    req.tv_sec = 0;             // 秒数
    req.tv_nsec = 500000000;    // 500000000 纳秒 = 0.5 秒
    while(1)
    {
        printf("clock_nanosleep function delay 0.5s\n");
        // 实时时钟   相对时间   延时时间   返回参
        clock_nanosleep(CLOCK_MONOTONIC, 0, &req, &rem);
    }
}
