#include "main.hpp"

// sleep()�������� -- ����Ϊ��λ��ʱ
void sleepTest()
{
    while (1)
    {
        printf("sleep function delay 1s\n");
        sleep(1);
    }
}

// usleep()�������� -- ��΢��Ϊ��λ��ʱ
void usleepTest()
{
    while (1)
    {
        printf("usleep function delay 0.5s\n");
        usleep(500000); // ��ʱ 0.5 ��
    }
}

// nanosleep()�������� -- ������Ϊ��λ��ʱ
void nanosleepTest()
{
    struct timespec req;    // ��Ҫ��ʱ��ʱ�������
    struct timespec rem;    // ��������Σ����������źŴ�ϣ�ʣ��ʱ�䷵�ص��ò���
    req.tv_sec = 0;             // ����
    req.tv_nsec = 500000000;    // 500000000 ���� = 0.5 ��
    while(1)
    {
        printf("nanosleep function delay 0.5s\n");
        nanosleep(&req, &rem);
    }
}

// clock_nanosleep()�������� -- ������Ϊ��λ��ʱ
void clock_nanosleepTest()
{
    struct timespec req;    // ��Ҫ��ʱ��ʱ�������
    struct timespec rem;    // ��������Σ����������źŴ�ϣ�ʣ��ʱ�䷵�ص��ò���
    req.tv_sec = 0;             // ����
    req.tv_nsec = 500000000;    // 500000000 ���� = 0.5 ��
    while(1)
    {
        printf("clock_nanosleep function delay 0.5s\n");
        // ʵʱʱ��   ���ʱ��   ��ʱʱ��   ���ز�
        clock_nanosleep(CLOCK_MONOTONIC, 0, &req, &rem);
    }
}
