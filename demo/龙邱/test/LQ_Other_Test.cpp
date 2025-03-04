#include "main.hpp"

// ����֡��
void CalculateFrameRate()
{
    static bool flags = true;       // �жϵ�һ�ν��뺯����־λ
    static uint8_t fps_count = 0;   // ֡���ۼ�
    static double start_time = 0;   // ��ʼʱ��
    static double end_time = 0;     // ����ʱ��
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

// ʱ�����ӡ����
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

// ��ȡ��ǰ΢��ֵ
void GetCurrentMillisecond()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("%s: %lld\n", __func__, (long long)(ts.tv_sec) * 1000000LL + (long long)(ts.tv_nsec) / 1000LL);
}

// ��ȡ��ǰ΢��ֵ
void printMicTimestamp()
{
    struct timeval ts;
    gettimeofday(&ts, NULL);
    printf("%s: %lld\n", __func__, (long long int)ts.tv_sec * 1000000LL + (long long int)ts.tv_usec);
}
