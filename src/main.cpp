#include "main.h"
/* ----------------------------------------主函数---------------------------------------- */
int main()
{
    try // 异常处理
    {
        init();
        // run();

        /* 创建线程 */
        std::thread opencv(opencv_thread);             // opencv线程
        std::thread right_motor(right_pid_pwm_thread); // 右轮控制线程
        std::thread left_motor(left_pid_pwm_thread);   // 左轮控制线程
        std::thread servo(servo_pid_pwm_thread);       // 舵机控制线程
        std::thread fans(fans_pwm_thread);             // 负压风扇控制线程
        std::thread imu(imu_thread);                   // 陀螺仪数据处理线程
        std::thread gpio(gpio_thread);                 // GPIO控制线程
        std::thread debugi(debugi_thread);             // 调试输入线程
        std::thread debugo(debugo_thread);             // 调试输出线程
        std::thread car(car_main_control_thread);      // 小车控制线程

        /* 等待线程结束 */
        car.join();
        opencv.join();
        right_motor.join();
        left_motor.join();
        servo.join();
        fans.join();
        imu.join();
        gpio.join();
        debugi.join();
        debugo.join();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        project_manage(-1);
    }
    return 0;
}

void init()
{
    signal(SIGINT, project_manage); // 设置进程终止处理函数
    /* JSON参数获取 */
    my_sync.ConfigData_SYNC(Data_Path_p, Function_EN_p);
    JSON_functionConfigData = Function_EN_p->JSON_FunctionConfigData_v[0];
    JSON_trackConfigData = Data_Path_p->JSON_TrackConfigData_v[0];

    /* 屏幕初始化 */
    ips200_init("/dev/fb0"); // 340*240
    ips200_show_gray_image(0, 0, gImage_seekfree_logo, 240, 180);

    /* 陀螺仪初始化 */
    imu_get_dev_info();

    /* 摄像头初始化 */
    if (!CameraInit(Camera, JSON_functionConfigData.Camera_EN, 120))
        // project_manage(-1);

        std::cout << "\33[33m" << PROGRAM_NAME << ":\33[0m init complete " << std::endl;
}

void car_main_control_thread()
{
    while (running)
    {
        // CameraImgGet(Img_Store_p, running);
        // my_img_process.ImgCompress(Img_Store_p->Img_Color, JSON_functionConfigData.ImgCompress_EN); // 图像压缩
        // my_img_process.ImgPrepare(Img_Store_p, Data_Path_p, Function_EN_p);                         // 图像预处理
        // ImgPathSearch(Img_Store_p, Data_Path_p);                                                    // 路径寻线
        // ImgSideSearch(Img_Store_p, Data_Path_p);                                                    // 边线八邻域寻线

        // Img_Store_p->ImgNum++; // 图像帧数
        // sender_ready == false ? sender_ready = true : sender_ready = sender_ready;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
