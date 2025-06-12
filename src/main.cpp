#include "headfile.h"

int main()
{
    try
    {
        init();

        std::cout << "wait key-1 to start............" << std::endl;
        while (!key1.readValue())
            ;

        std::thread opencv(opencv_thread);           // opencv线程  包含无畸变处理
        std::thread img_process(img_process_thread); // 图像处理线程
        std::thread car(car_main_control_thread);    // 小车控制线程
        std::thread hardware(hardware_control);      // 硬件控制线程
        std::thread debug1(debug1_thread);           // 高速调试线程
        std::thread debug2(debug2_thread);           // 低速调试线程
        std::thread IO(IO_thread);                   // IO线程

        IO.join();
        car.join();
        img_process.join();
        opencv.join();
        debug1.join();
        hardware.join();
        debug2.join();
        std::cout << "程序正常退出" << std::endl;
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

    l_target = 6;
    r_target = 6;

    std::cout << "Camera Init result: " << (CameraInit(Camera, 2, 100) ? "success,!" : "failed!!!") << std::endl;
    std::cout << "IMU660RA Init result: " << (imu_get_dev_info() ? "success,!" : "failed!!!") << std::endl;
    ips200_init("/dev/fb0");

    ips200_show_string(10, 10, "Init success!!! wait key-1 to start...");

#if IMG_SEND
    std::cout << "ImgSender Init result: " << (ImgSender.init(DST_IP, UDP_PORT) ? "success,!" : "failed!!!") << std::endl;
#endif
}

void img_process_thread()
{
    std::cout << "img_process_thread开始进入循环" << std::endl;
    while (running)
    {
        control_v++;
        CameraImgGet(Img_Store_pp, running);
        // my_img_process.ImgCompress(Img_Store_pp->Img_Color, JSON_functionConfigData.ImgCompress_EN); // 图像压缩
        ImgPrepare(Img_Store_pp); // 图像预处理

        get_image = Img_Store_pp->Img_OTSU.clone();
        cv::cvtColor(get_image, get_color, cv::COLOR_GRAY2BGR);

        // std::cout<<get_image.cols<<","<<get_image.rows<<std::endl;
        Center_line_deal(); ////中线处理 和 十字处理
        youhuandao();
        zuohuandao();
        Mid_Line_Repair(sousuojieshuhang); ////优化center[]
        // Get_Point = Point_Weight();////center[]影响输入  输出偏度18~184
        ////S_D5_Duty = PlacePID_Control(&S_D5_PID, zhongzhi, Point);

        chujiebaohu();
        if (chujie_num > 90 && chujie == 0 && park_flag != 2)
        {
            chujie = 1;
            // running=0;
        }
        cv::Mat haha;

        for (int iqq = 68; iqq > sousuojieshuhang; iqq--)
        {
            cv::circle(get_color, cv::Point(left_line[iqq] + 2, iqq), 1, cv::Scalar(0, 0, 150), -1);        // -1 表示实心圆
            cv::circle(get_color, cv::Point(right_line[iqq] - 2, iqq), 1, cv::Scalar(0, 0, 150), -1);       // -1 表示实心圆
            cv::circle(get_color, cv::Point(Left_Line_New[iqq] + 2, iqq), 1, cv::Scalar(0, 255, 255), -1);  // -1 表示实心圆
            cv::circle(get_color, cv::Point(Right_Line_New[iqq] - 2, iqq), 1, cv::Scalar(0, 255, 255), -1); // -1 表示实心圆

            // cv::circle(get_color, cv::Point(center_th[iqq]-2, iqq), 1, cv::Scalar(0,0,255), -1);  // -1 表示实心圆
            cv::circle(get_color, cv::Point(center[iqq] - 2, iqq), 1, cv::Scalar(0, 255, 0), -1); // -1 表示实心圆
        }
        for (int y = 68; y > 20; y--)
        {
            if (Right_Add2[y] == 1)
                cv::circle(get_color, cv::Point(Right_Line_New2[y], y), 1, cv::Scalar(255, 0, 255), -1); // -1 表示实心圆
            if (Left_Add2[y] == 1)
                cv::circle(get_color, cv::Point(Left_Line_New2[y], y), 1, cv::Scalar(255, 0, 255), -1); // -1 表示实心圆
        }
        cv::circle(get_color, cv::Point(right_line[r_start], r_start), 1, cv::Scalar(255, 255, 255), -1); // -1 表示实心圆
        cv::circle(get_color, cv::Point(left_line[l_start], l_start), 1, cv::Scalar(255, 255, 255), -1);  // -1 表示实心圆

        cv::circle(get_color, cv::Point(right_line[r_start], r_start), 1, cv::Scalar(0, 0, 255), -1); // -1 表示实心圆
        cv::circle(get_color, cv::Point(left_line[l_start], l_start), 1, cv::Scalar(0, 0, 255), -1);  // -1 表示实心圆

        if (findleftdownguai == 1)
        {
            cv::circle(get_color, cv::Point(left_turn_down[1], left_turn_down[0]), 4, cv::Scalar(255, 0, 0), 2); // -1 表示实心圆
        }
        if (findrightdownguai == 1)
        {
            cv::circle(get_color, cv::Point(right_turn_down[1], right_turn_down[0]), 4, cv::Scalar(255, 0, 0), 2); // -1 表示实心圆
        }
        if (findrightupguai == 1)
        {
            cv::circle(get_color, cv::Point(right_turn_up[1], right_turn_up[0]), 4, cv::Scalar(255, 0, 0), 2);
        }
        if (findleftupguai == 1)
        {
            cv::circle(get_color, cv::Point(left_turn_up[1], left_turn_up[0]), 4, cv::Scalar(255, 0, 0), 2);
        }

        // if (sousuojieshuhang < 8 && Width[50] < 100)

        haha = get_color.clone();
        // cv::resize(get_color, haha, cv::Size(188, 70));
        //  cv::putText(haha, std::to_string(white_num_col_line), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

        // cv::putText(haha, std::to_string(findleftdownguai), cv::Point(10, 40), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);
        // cv::putText(haha, std::to_string(findrightdownguai), cv::Point(30, 40), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

        // cv::putText(haha, std::to_string(k_center), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

        cv::putText(haha, std::to_string(left_huan_num), cv::Point(10, 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 200, 0), 1, cv::LINE_AA);

        cv::putText(haha, std::to_string(Left_Add_num), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);
        cv::putText(haha, std::to_string(Right_Add_num), cv::Point(30, 50), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

        cv::putText(haha, std::to_string(zuodiuxianshu), cv::Point(30, 70), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);
        // //float angles=-(Get_Point-87)*((1700000-1346000)/100)+(1700000+1346000)/2;
        // int last_sp_duty;
        // sp_duty=(int)(angles*0.6)+(int)(last_sp_duty*0.4);
        // last_sp_duty=sp_duty;
        // cv::putText(haha, std::to_string(sp_duty), cv::Point(30, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);
        {
            std::lock_guard<std::mutex> lock(image_mutex); // 锁住图像数据，确保线程安全
            image_to_send = haha.clone();                  // 拷贝图像数据
            std::lock_guard<std::mutex> ulock(image_mutex);
            // image_to_send = canvas.clone(); // 拷贝图像数据
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
