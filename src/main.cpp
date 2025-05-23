#include "main.h"



/* ----------------------------------------主函数---------------------------------------- */

UdpSender g_udp_sender;
std::mutex image_mutex; // 定义一个互斥锁
cv::Mat image_to_send;  // 需要在多个线程之间共享的图像数据

int main()
{
    try // 异常处理
    {
        init();
        

        //run();

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
    /* JSON参数获取 */
    my_sync.ConfigData_SYNC(Data_Path_p, Function_EN_p);
    JSON_functionConfigData = Function_EN_p->JSON_FunctionConfigData_v[0];
    JSON_trackConfigData = Data_Path_p->JSON_TrackConfigData_v[0];
    #if SPEED_MODE == 10
        JSON_trackConfigData.Forward=50;
    #else
        JSON_trackConfigData.Forward=48;
    #endif

    std::cout << "start running..." << std::endl;
                signal(SIGINT, project_manage); // 设置进程终止处理函数
                running = true;

    // /* 屏幕初始化 */
    // ips200_init("/dev/fb0"); // 340*240
    // ips200_show_gray_image(0, 0, gImage_seekfree_logo, 240, 180);

    /* 陀螺仪初始化 */
    // imu_get_dev_info();

    //读取映射表,畸变参数  永远不变 

    /* 摄像头初始化 */
    if (!CameraInit(Camera, JSON_functionConfigData.Camera_EN, 120))
        project_manage(-1);

    /* 电机初始化 */
    l_target = 10;
    r_target = 10;

    if (!g_udp_sender.init(DST_IP, 8080)) {
        std::cerr << "图传UDP初始化失败" << std::endl;
    }


    std::cout << "\33[33m" << PROGRAM_NAME << ":\33[0m init complete " << std::endl;
}

unsigned int image__num = 0;

void car_main_control_thread()
{
    while (running)
    {
        image__num++;
        CameraImgGet(Img_Store_p, running);
        //my_img_process.ImgCompress(Img_Store_p->Img_Color, JSON_functionConfigData.ImgCompress_EN); // 图像压缩
        my_img_process.ImgPrepare(Img_Store_p, Data_Path_p, Function_EN_p);                         // 图像预处理
        ImgPathSearch(Img_Store_p, Data_Path_p);                                                    // 路径寻线
        ImgSideSearch(Img_Store_p, Data_Path_p);  

        Data_Path_p->get_dandiao=CheckPattern(Data_Path_p,10,3);
        right_white_hang=GetWhiteEnd(Data_Path_p);


        // cv::line((Img_Store_p->Img_Track), cv::Point(0,(Data_Path_p->SideCoordinate_Eight[20][3])), cv::Point(159,(Data_Path_p->SideCoordinate_Eight[20][3])), cv::Scalar(255, 0, 0), 2); 
        // std::string TextLoop_Kind_EN[8] = {"CAMERA_CATCH_LOOP", "JUDGE_LOOP", "COMMON", "R_CIRCLE", "L_CIRCLE", "ACROSS"};
        // putText((Img_Store_p->Img_Track), TextLoop_Kind_EN[int(Function_EN_p->Loop_Kind_EN)], cv::Point(30, 15), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        // std::string TextCircleKind[6] = {"IN_PREPARE", "IN", "OUT_PREPARE", "OUT", "OUT_2_STRIGHT", "INIT"};


        // putText((Img_Store_p->Img_Track), (std::to_string)(Img_Store_p->ImgNum), cv::Point(170, 15), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        // putText((Img_Store_p->Img_Track), (std::to_string)(NumSearch), cv::Point(250, 15), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);

        // putText((Img_Store_p->Img_Track), TextCircleKind[int(Data_Path_p->Circle_Track_Step)], cv::Point(30, 90), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        // putText((Img_Store_p->Img_Track), "Left_guai: " + std::to_string(Data_Path_p->InflectionPointNum[0]), cv::Point(0,30), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        // putText((Img_Store_p->Img_Track), "Left_wan: " + std::to_string(Data_Path_p->BendPointNum[0]), cv::Point(0,50), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        // putText((Img_Store_p->Img_Track), "Right_guai: " + std::to_string(Data_Path_p->InflectionPointNum[1]), cv::Point(180,30), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        // putText((Img_Store_p->Img_Track), "Right_wan: " + std::to_string(Data_Path_p->BendPointNum[1]), cv::Point(180,50), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        // putText((Img_Store_p->Img_Track), "Vector_Left: " + std::to_string(Data_Path_p->Vector_Add_Unit_Dir[0]), cv::Point(0,70), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        // putText((Img_Store_p->Img_Track), "Vector_Right: " + std::to_string(Data_Path_p->Vector_Add_Unit_Dir[1]), cv::Point(150,70), cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);


        // putText((Img_Store_p->Img_Track), "L_f_s: " + std::to_string(Data_Path_p->SideCoordinate[(JSON_trackConfigData.Forward) - (JSON_trackConfigData.Path_Search_Start)][0]), cv::Point(50,180), cv::FONT_HERSHEY_COMPLEX, 0.6, cv::Scalar(0, 255, 0), 1);
        // putText((Img_Store_p->Img_Track), "R_f_s: " + std::to_string(Data_Path_p->SideCoordinate[(JSON_trackConfigData.Forward) - (JSON_trackConfigData.Path_Search_Start)][2]), cv::Point(50,220), cv::FONT_HERSHEY_COMPLEX, 0.6, cv::Scalar(0, 255, 0), 1);
        // //putText((Img_Store_p->Img_Track), "a_f_s: " + std::to_string(Data_Path_p->SideCoordinate[2][0]), cv::Point(130,180), cv::FONT_HERSHEY_COMPLEX, 0.6, cv::Scalar(0, 255, 0), 1);
        //putText((Img_Store_p->Img_Track), "I_f_s: " + std::to_string(Data_Path_p->SideCoordinate[2][2]), cv::Point(130,220), cv::FONT_HERSHEY_COMPLEX, 0.6, cv::Scalar(0, 255, 0), 1);

        //std::string TextPrevious_Circle_Kind[7] = {"STRIGHT_TRACK", "BEND_TRACK", "R_CIRCLE_TRACK_OUTSIDE", "R_CIRCLE_TRACK_INSIDE", "L_CIRCLE_TRACK_OUTSIDE", "L_CIRCLE_TRACK_INSIDE","ACROSS_TRACK"};
        

        Img_Store_p->ImgNum++; // 图像帧数

        // 赛道状态机决策循环
        Function_EN_p->Loop_Kind_EN = my_judge.TrackKind_Judge(Img_Store_p, Data_Path_p, Function_EN_p); // 切换至赛道循环

        if (Function_EN_p->Loop_Kind_EN == COMMON_TRACK_LOOP)
        {
            // if (Data_Path_p->Circle_Track_Step == IN_PREPARE)
            // {
            //     CircleTrack_Step_IN_Prepare_Stright(Img_Store_p, Data_Path_p); // 准备入环补线
            // }
            // if (Data_Path_p->Circle_Track_Step == OUT_2_STRIGHT)
            // {
            //     Circle2CommonTrack(Img_Store_p, Data_Path_p); // 出环转直线补线
            // }
            ImgPathSearch(Img_Store_p, Data_Path_p);   // 赛道路径线寻线
            my_judge.ServoDirAngle_Judge(Data_Path_p); // 舵机角度计算
            my_judge.MotorSpeed_Judge(Img_Store_p, Data_Path_p); // 电机速度决策
        }
        if( Function_EN_p -> Loop_Kind_EN == L_CIRCLE_TRACK_LOOP || Function_EN_p -> Loop_Kind_EN == R_CIRCLE_TRACK_LOOP )
        {
            // switch(Data_Path_p -> Circle_Track_Step)
            // {
            //     case IN_PREPARE:
            //     {
            //         CircleTrack_Step_IN_Prepare(Img_Store_p,Data_Path_p);   // 准备入环补线
            //         break;
            //     }
            //     case IN:
            //     {
            //         CircleTrack_Step_IN(Img_Store_p,Data_Path_p);   // 入环补线
            //         break;
            //     }
            //     case OUT:
            //     {
            //         CircleTrack_Step_OUT(Img_Store_p,Data_Path_p);   // 出环补线
            //         break;
            //     }
            // }
            ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
            my_judge.ServoDirAngle_Judge(Data_Path_p); // 舵机角度计算
            my_judge.MotorSpeed_Judge(Img_Store_p,Data_Path_p);    // 电机速度决策
        
        }
        if (Function_EN_p->Loop_Kind_EN == ACROSS_TRACK_LOOP)
        {
            AcrossTrack(Img_Store_p, Data_Path_p);     // 十字赛道补线
            ImgPathSearch(Img_Store_p, Data_Path_p);   // 赛道路径线寻线
            my_judge.ServoDirAngle_Judge(Data_Path_p); // 舵机角度计算
            my_judge.MotorSpeed_Judge(Img_Store_p, Data_Path_p); // 电机速度决策
            // Function_EN_p->Loop_Kind_EN = CAMERA_CATCH_LOOP;  // 切换至串口发送循环
        }


        
        //白色终止行
        cv::circle(Img_Store_p->Img_Track, cv::Point(Data_Path_p->SideCoordinate_Eight[right_white_hang][2], Data_Path_p->SideCoordinate_Eight[right_white_hang][3]), 5, cv::Scalar(0, 0, 0), 2); 
        
        
        //拐点绘制
        my_img_process.ImgInflectionPointDraw(Img_Store_p, Data_Path_p); 
        //前瞻画
        my_img_process.ImgForwardLine(Img_Store_p, Data_Path_p);
        //单调性变化点
        if(Data_Path_p->get_dandiao)
            cv::circle(Img_Store_p->Img_Track, cv::Point(Data_Path_p->SideCoordinate[Data_Path_p->get_dandiao][2], Data_Path_p->SideCoordinate[Data_Path_p->get_dandiao][3]), 5, cv::Scalar(0, 0, 0), 3); 
        //最远路径点
        cv::circle(Img_Store_p->Img_Track, cv::Point(Data_Path_p->TrackCoordinate[NumSearch-1][0], Data_Path_p->TrackCoordinate[NumSearch-1][1]), 3, cv::Scalar(255, 0, 0), 1); 

        {
            std::lock_guard<std::mutex> lock(image_mutex);  // 锁住图像数据，确保线程安全
            resize(Img_Store_p->Img_Track, Img_Store_p->Img_Send, cv::Size(320, 240));
            image_to_send = Img_Store_p->Img_Send.clone(); // 拷贝图像数据
            //image_to_send = canvas.clone(); // 拷贝图像数据
        }
        
        //  auto now = std::chrono::system_clock::now();
        // auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        //     now.time_since_epoch()
        // ).count();
        // std::cout << "Timestamp (ms): " << timestamp << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}