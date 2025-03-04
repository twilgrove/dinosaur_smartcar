#include "main.hpp"

/*
        如果使用的是 4.19 版本内核，那么 ADC 驱动被没有被载入，
    可以参考龙邱科技B站视频来配置这一环节。
    视频链接：https://www.bilibili.com/video/BV1C2KHekEEJ
*/

#define ADC_0 "in_voltage0_raw"
#define ADC_1 "in_voltage1_raw"
#define ADC_2 "in_voltage2_raw"
#define ADC_3 "in_voltage3_raw"
#define ADC_4 "in_voltage4_raw"
#define ADC_5 "in_voltage5_raw"
#define ADC_6 "in_voltage6_raw"
#define ADC_7 "in_voltage7_raw"

#define ADC_SCALE "/sys/bus/iio/devices/iio:device0/in_voltage_scale"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void AdcFunTest()
 * @功能说明：ADC 功能测试
 * @参数说明：无
 * @函数返回：无
 * @调用方法：AdcFunTest();
 * @备注说明：想要尝试不同 ADC 引脚，修改 40 行中 string(ADC_0) 的宏即可
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void AdcFunTest()
{
    int fd_scale = open(ADC_SCALE, O_RDONLY);
    if(fd_scale < 0)
    {
        perror("open1");
        return;
    }
    char scale[10] = {0};
    if (read(fd_scale, scale, sizeof(scale)) != sizeof(scale))
    {
        perror("read1");
        return;
    }
    close(fd_scale);

    double scaleNum = strtod(scale, NULL);
    printf("scaleNum = %f\n", scaleNum);
    string str = "/sys/bus/iio/devices/iio:device0/" + string(ADC_0);
    char num[4] = {0};
    double Num = 0;
    while (1)
    {
        int fd_adc = open(str.c_str(), O_RDONLY);
        if (fd_adc < 0)
        {
            perror("open2");
            return;
        }
        if (read(fd_adc, num, sizeof(num)) <= 0)
        {
            perror("read2");
        }
        close(fd_adc);
        Num = strtod(num, NULL);
        printf("Num = %f, ADC = %f\n", Num, scaleNum*Num/1000);
        memset(num, 0, sizeof(num));
        sleep(1);
    }

    // FILE *file = fopen(str.c_str(), "r");
    // if (file == NULL)
    // {
    //     perror("fopen");
    //     return;
    // }
    // while(1)
    // {
    //     // 将文件指针移动到文件开头
    //     if (fseek(file, 0, SEEK_SET) != 0)
    //     {
    //         perror("fseek");
    //         return;
    //     }
    //     if (fread(num, sizeof(num), 1, file) <= 0)
    //     {
    //         perror("fread");
    //         return;
    //     }
    //     Num = strtod(num, NULL);
    //     printf("Num = %f, ADC = %f\n", Num, scaleNum*Num/1000);
    //     memset(num, 0, sizeof(num));
    //     sleep(1);
    // }
}
