#include "headfile.h"
#include "camera.h"
#include "thread.h"

int running = 0;
cv::VideoCapture Camera;
extern unsigned char image_use[70][188];

#define UDP_PORT 8080
#define DST_IP "192.168.31.16"
#define MAX_PACKET_SIZE 1024

int main()
{
    try
    {
        init();
        std::cout << "CameraInit 结果: "
                  << (CameraInit(Camera, 2, 100) ? "成功" : "失败")
                  << std::endl;
        std::thread opencv(opencv_thread);           // opencv线程  包含无畸变处理
        std::thread car(car_main_control_thread);    // 小车控制线程
        std::thread debugi(debugi_thread);           // 调试输入线程
        std::thread motor_servo(motor_servo_thread); // 电机舵机线程
        std::cout << "线程创建完成,等待join..." << std::endl;

        car.join();
        opencv.join();
        debugi.join();
        motor_servo.join();
        std::cout << "程序正常退出" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        running = 0;
    }
    return 0;
}
void init()
{
    signal(SIGINT, project_manage); // 设置进程终止处理函数
    running = 1;

    // // 视频录制初始化
    // std::string videoPath = "C:/Users/azw20/Desktop/智能車/独立代码/recorded/track_video.avi";
    // cv::Size frameSize(320, 240);
    // video_writer = new cv::VideoWriter(videoPath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 20, frameSize, true);
    // if (!video_writer->isOpened())
    // {
    //     std::cerr << "无法打开视频文件进行写入！" << std::endl;
    //     return;
    // }

    if (!g_udp_sender.init(DST_IP, 8080))
    {
        std::cerr << "图传UDP初始化失败" << std::endl;
    }
}

int self_count;
void car_main_control_thread()
{
    std::cout << "car_main_control_thread开始进入循环" << std::endl;
    while (running)
    {
        CameraImgGet(Img_Store_pp, running);
        // my_img_process.ImgCompress(Img_Store_pp->Img_Color, JSON_functionConfigData.ImgCompress_EN); // 图像压缩
        ImgPrepare(Img_Store_pp); // 图像预处理

        cv::Mat get_image = Img_Store_pp->Img_OTSU.clone();
        cv::Mat get_color;
        cv::cvtColor(get_image, get_color, cv::COLOR_GRAY2BGR);

        // std::cout<<get_image.cols<<","<<get_image.rows<<std::endl;
        k_center = 0;
        qvlv_quanju_right = qvlv_quanju_left = qulv_jinduan_right = qulv_jinduan_left = qulv_yuandaun_right = qulv_yuandaun_left = 0;
        k_left = 0;
        k_right = 0;
        Width_Min = 90;
        // th_y=0;
        whitenum = 0;
        right_turn_down[0] = 69;
        left_turn_down[0] = 69;
        right_turn_down[1] = 187;
        left_turn_down[1] = 1;
        // right_turn_up[0]=0;
        //  left_turn_up[0]=0;
        xielv_1eft2 = 80;
        xielv_right2 = 80;
        findleftdownguai = 0;
        findrightdownguai = 0;
        findrightupguai = 0;
        findleftupguai = 0;
        times = 0;
        Foresight_Left = 0, Foresight_Right = 186;
        youdiuxianshu = 0;
        zuodiuxianshu = 0;
        Left_Add_Start = 0;
        Right_Add_Start = 0;
        Left_Add_End = 0;
        Right_Add_End = 0;
        white_num_col_max = 0;
        Left_Add_num = 0, Right_Add_num = 0;
        Left_Add_num2 = 0, Right_Add_num2 = 0;
        white_num_col_min = 69;
        l_start = 68;
        r_start = 68;

        self_count = 0;
        for (int y = 0; y < 70; ++y)
        {
            for (int x = 0; x < 188; ++x)
            {
                image_use[y][x] = Img_Store_pp->Img_OTSU.at<uchar>(y, x); // 值为 0 或 255
                // std::cout<<image_use[y][x]<<std::endl;
                // std::cout<<Img_Store_pp->Img_OTSU.cols<<","<<Img_Store_pp->Img_OTSU.rows<<std::endl;
            }
        }

        for (int ql = 0; ql <= 69; ql++) // 清零函数
        {
            left_line[ql] = 2;     ////存储左边界线的列坐标
            Left_Add[ql] = 0;      ////标记左边界是否需要基础补线
            Left_Add2[ql] = 0;     ////标记左边界是否需要高级补线（动态斜率补线标志）
            Left_Line_New[ql] = 2; ////存储修复后的左边界线（最终使用的左边界）
        }
        for (int ql = 0; ql <= 187; ql++) // 清零函数guaidian
        {
            white_num_col[ql] = 0; ////白列白点数
        }
        for (int ql = 0; ql <= 69; ql++)
        {
            right_line[ql] = 184;     ////存储右边界线的列坐标
            Right_Line_New[ql] = 184; ////存储修复后的右边界线（最终使用的右边界）
            center[ql] = 89;          // 每行中线点
            Right_Add[ql] = 0;        ////标记右边界是否需要基础补线
            Right_Add2[ql] = 0;       ////标记右边界是否需要高级补线（动态斜率补线标志）
        }
        unsigned int temp = 0;

        if (park_flag == 0)
        {
            for (int x = 184; x >= 101; x--)
            {
                for (int y = 69; y >= 0; y--)
                {
                    if (image_use[y][x] == 0)
                    {
                        break;
                    }
                    else
                    {
                        white_num_col[x]++;
                    }
                }
                if (white_num_col[x] > white_num_col_max)
                {
                    white_num_col_max = white_num_col[x];
                    white_num_col_line = x;
                }
            }
            for (int x = 18; x <= 100; x++)
            {
                for (int y = 69; y >= 0; y--)
                {
                    if (image_use[y][x] == 0)
                    {
                        break;
                    }
                    else
                    {
                        white_num_col[x]++;
                    }
                }
                if (white_num_col[x] > white_num_col_max)
                {
                    white_num_col_max = white_num_col[x];
                    white_num_col_line = x;
                } // k_right
            }
        }

        int y = 0;
        ////最长白列法寻找边界
        for (y = 68; y > 1; y--) // x是减59—56  num是加0—4   ////白点为边界
        {
            ////最长摆列法右边界
            for (int x = white_num_col_line; x <= 184; x++) // 中间向右找跳变
            {
                if (image_use[y][x - 1] == 255 && image_use[y][x] == 255 && image_use[y][x + 1] == 0 && image_use[y][x + 2] == 0) // 两个连续黑点触发
                {
                    right_line[y] = x;
                    Right_Line_New[y] = x;
                    Right_Line_New2[y] = x;
                    break;
                }
                else if (x == 184) // 到最右边了都没扫到黑点a
                {
                    right_line[y] = x;
                    Right_Line_New[y] = x;
                    Right_Line_New2[y] = x;
                    Right_Add[y] = 1;
                    break;
                }
            }
            ////最长摆列法左边界
            for (int x = white_num_col_line; x >= 1; x--) // 中间向左找跳变
            {
                if (image_use[y][x - 2] == 0 && image_use[y][x - 1] == 0 && image_use[y][x] == 255 && image_use[y][x + 1] == 255) // 两个连续黑点触发
                {
                    left_line[y] = x;
                    Left_Line_New[y] = x;
                    Left_Line_New2[y] = x;
                    break;
                }
                else if (x == 3) // 到最左边了都没扫到黑点
                {

                    left_line[y] = x;
                    Left_Line_New[y] = x;
                    Left_Line_New2[y] = x;
                    Left_Add[y] = 1;

                    break;
                }
            }
            //   Half_width[y]=abs(right_line[y]-left_line[y])/2;
            Width[y] = abs(right_line[y] - left_line[y]); ////Width赛道宽度
            // std::cout<<"Y:"<<y<<"   "<<right_line[y] - left_line[y]<<std::endl;
            ////中线处理
            if (left_line[y] <= 3 && right_line[y] < 184)
            {
                if (right_line[y] - Half_width_yuanshi[y] <= 3)
                    center_th[y] = 3;
                else
                    center_th[y] = right_line[y] - Half_width_yuanshi[y];
            }
            else if (left_line[y] > 3 && right_line[y] >= 184)
            {
                if (left_line[y] + Half_width_yuanshi[y] >= 184)
                    center_th[y] = 184;
                else
                    center_th[y] = left_line[y] + Half_width_yuanshi[y];
            }
            else
                center_th[y] = (left_line[y] + right_line[y]) / 2;

            ////赛道变宽判断
            if (Width[y] >= Width[y + 1] || (Width[y] >= Width_Min))
            {

                if (Left_Add2[y + 1])
                {
                    if (left_line[y] < Left_Line_New2[y + 1] - 1) // 与前一行的左边界实线比较
                    {
                        Left_Add2[y] = 1;
                    }
                }
                else // 前一行没有补线
                {
                    if (left_line[y] < left_line[y + 1] - 1) // 与前一行的左边界实线比较
                    {
                        Left_Add2[y] = 1; ////更新Left_Add2[]
                    }
                }
                if (Right_Add2[y + 1])
                {
                    if (right_line[y] > Right_Line_New2[y + 1] + 1)
                    {
                        Right_Add2[y] = 1; ////更新Right_Add2[]
                    }
                }
                else // 前一行右边界没有补线
                {
                    if (right_line[y] > right_line[y + 1] + 1)
                    {
                        Right_Add2[y] = 1;
                    }
                }
            }
            if (Left_Add2[y]) // 左边需要补线
            {

                if (y < 65) // 因为有下面+6限制
                {
                    if (!Left_Add_Start) // 如果还没有记录开始补线位置
                    {
                        Left_Add_Start = y; // 记录左边界补线开始位置
                    }
                    Add_Slope = 1.0 * (left_line[Left_Add_Start + 6] - left_line[Left_Add_Start + 1]) / 5; // 计算能识别的前几行图像斜率

                    std::cout << "left_line[Left_Add_Start + 6]: " << left_line[Left_Add_Start + 6] << "   left_line[Left_Add_Start + 1]: " << left_line[Left_Add_Start + 1] << "   Add_Slope: " << Add_Slope << std::endl;
                    if (Add_Slope > 0) // 限幅
                    {
                        Add_Slope = 0;
                    }
                    temp = (char)((y - (Left_Add_Start + 1)) * Add_Slope + left_line[Left_Add_Start + 1]); // 通过斜率推算补线的位置
                    Left_Last_Slope = Add_Slope;                                                           // 更新上次左边界斜率

                    Left_Line_New2[y] = range_protect(temp, 2, 184); // 不直接修改边界，只保存在补线数组里
                    std::cout << "Left_Add_Start: " << Left_Add_Start << "  left:  " << y << "   " << temp << std::endl;
                    std::cout << "get_first: " << (y - (Left_Add_Start + 1)) * Add_Slope << std::endl;
                }
                /* 第一次补线，只记录，不在图像上显示 */
                //
            }
            ////右补线
            if (Right_Add2[y]) // 右边需要补线
            {
                if (y < 65)
                {
                    if (!Right_Add_Start) // 如果还没有记录开始补线位置
                    {
                        Right_Add_Start = y; // 记录左边界补线开始位置
                    }

                    Add_Slope = 1.0 * (right_line[Right_Add_Start + 6] - right_line[Right_Add_Start + 1]) / 4; // 计算能识别的前几行图像斜率

                    if (Add_Slope < 0) // 限幅
                    {
                        Add_Slope = 0;
                    }
                    temp_r = (char)((y - (Right_Add_Start + 1)) * Add_Slope + right_line[Right_Add_Start + 1]); // 通过斜率推算补线的位置
                    Right_Last_Slope = Add_Slope;                                                               // 更细上次右边界斜率

                    Right_Line_New2[y] = range_protect(temp_r, 2, 184); // 不直接修改边界，只保存在补线数组里
                }
            }

            ////计算理论补线后重新计算赛道宽度
            if (Left_Add2[y] && Right_Add2[y]) // 两边都需要补线
            {
                Width[y] = Right_Line_New2[y] - Left_Line_New2[y]; // 重新计算本行赛道宽度
            }
            else // 不需要补线或只有一边需要补线
            {
                if (Left_Add2[y]) // 此处最多只有一边会需要补线
                {
                    Width[y] = right_line[y] - Left_Line_New2[y]; // 重新计算本行赛道宽度
                }
                else if (Right_Add2[y])
                {
                    Width[y] = Right_Line_New2[y] - left_line[y]; // 重新计算本行赛道宽度
                }
                else
                {
                    Width[y] = right_line[y] - left_line[y]; // 可能会有干扰，也重新计算一次赛道宽度
                }
                if (Width[y] < Width_Min)
                {
                    Width_Min = Width[y]; // 更新最小赛道宽度
                }
                if (left_line[y] > Foresight_Left) // 更新左边界最大值
                {
                    Foresight_Left = left_line[y];
                }
                if (right_line[y] < Foresight_Right) // 更新右边界最小值
                {
                    Foresight_Right = right_line[y];
                }
            }
            if (y >= 30 && Right_Add[y] == 1 && Left_Add[y] == 1)
                times++;
            ////两条边线相交
            if (abs(right_line[y] - left_line[y]) <= 1)
                break;
            ////边线交叉，中点为黑
            if (image_use[y][center_th[y]] == 0)
                break;
            ////搜索到终点
            if (y <= 1)
                break;
        }

        sousuojieshuhang = y + 2;

        ////统计左右边界丢线zuodiuxianshu l_start   youdiuxianshu r_start
        Cal_losttimes(sousuojieshuhang);

        ////统计需要动态斜率补线的右边界行数 Right_Add_num Left_Add_num
        for (y = 68; y > 20; y--)
        {
            if (Right_Add2[y] == 1)
                Right_Add_num++;
            if (Left_Add2[y] == 1)
                Left_Add_num++;
        }
        ////需要基础补线的左右边界行数 Right_Add_num2 Left_Add_num2
        for (y = 68; y > 48; y--)
        {
            if (Right_Add[y] == 1)
                Right_Add_num2++;
            if (Left_Add[y] == 1)
                Left_Add_num2++;
        }

        huihuan_num = 0; // 纵向较大白列>66的数目
        huandao_7 = 0;   //>68
        zhidao_num = 0;  //>41
        ////计算纵向较大白列huihuan_num的数目
        for (int x = right_line[68]; x >= left_line[68]; x--)
        {
            if (white_num_col[x] > 66)
                huihuan_num++;
        }
        ////计算纵向大白列zhidao_num和小白列huandao_7的数目
        for (int x = 118; x >= 80; x--)
        {
            if (white_num_col[x] > 68)
                zhidao_num++;
            if (white_num_col[x] > 41)
                huandao_7++;
        }
        if (l_start >= 55 || r_start >= 55)
        {
            find_leftdown_point(67, 15, 1); // 1是十字////left_turn_down[0]存储左拐点Y  left_turn_down[1]存储左拐点X
            find_rightdown_point(67, 15, 1);
        }
        regression(1, 20, 68);
        k_left = parameterB; ////左边界全局斜率拟合

        regression(2, 20, 68); ////右边界全局斜率拟合
        k_right = parameterB;

        // 直接处理的中线
        for (int iqq = 68; iqq > 20; iqq--)
        {

            cv::circle(get_color, cv::Point(center_th[iqq], iqq), 1, cv::Scalar(255, 0, 0), -1); // -1 表示实心圆
        }

        if (!left_huan_num && !right_huan_num)
        {
            // 如果找到左下或者右下拐点，此时检测左右线趋势

            // 时时注意拐点坐标第一个是行数+1，处理时要做减一处理
            if (left_turn_down[0] != 69 || right_turn_down[0] != 69)
            {
                if (left_turn_down[0] != 69 && right_turn_down[0] == 69) // 左下拐点存在而右下拐点不存在
                {
                    regression(1, left_turn_down[0] - 3, left_turn_down[0] + 2); // 左线
                    trend_of_left = parameterB;
                    regression(2, left_turn_down[0] - 3, left_turn_down[0] + 2); // 右线
                    trend_of_right = parameterB;
                }
                else if (right_turn_down[0] != 69 && left_turn_down[0] == 69) // 右下拐点存在而左下拐点不存在
                {
                    regression(1, right_turn_down[0] - 3, right_turn_down[0] + 2); // 左线
                    trend_of_left = parameterB;
                    regression(2, right_turn_down[0] - 3, right_turn_down[0] + 2); // 右线
                    trend_of_right = parameterB;
                }
                else if (left_turn_down[0] != 69 && left_turn_down[0] != 69) // 左右拐点均存在
                {
                    regression(1, left_turn_down[0] - 3, left_turn_down[0] + 2); // 左线
                    trend_of_left = parameterB;
                    regression(2, right_turn_down[0] - 3, right_turn_down[0] + 2); // 右线
                    trend_of_right = parameterB;
                }
            }
            if ((trend_of_left > 0 && trend_of_right < 0) || (trend_of_left < 0 && trend_of_right > 0))
                twolines_trend = 1; /////左右边界趋势相反（十字特征）
            else
                twolines_trend = 0; ////趋势冲突

            if ((left_turn_down[0] != 69 && twolines_trend == 1) || (youdiuxianshu >= 15 && left_turn_down[0] != 69))
            {

                findleftdownguai = 1; // 表示找到左下拐点了
            }
            else
                findleftdownguai = 0;
            if ((right_turn_down[0] != 69 && twolines_trend == 1) || (zuodiuxianshu >= 15 && right_turn_down[0] != 69))
            {

                findrightdownguai = 1; // 表示找到右下拐点了
            }
            else
                findrightdownguai = 0;

            regression(0, 58, 68); ////近端中线拟合
            ////通过近端中线拟合结果扩展到全局
            for (int j = 68; j >= 1; j--)
            {
                int jicun = (int)(parameterB * j + parameterA);
                if (jicun >= 185)
                    jicun = 185;
                else if (jicun <= 0)
                    jicun = 0;
                center_th[j] = (int)jicun;
            }
            ////中线有效性验证  有效中线点数whitenum  th_y有效中线断点处
            for (int y = 68; y > 0; y--)
            {
                if (image_use[y][center_th[y]] == 0)
                {
                    th_y = center_th[y];
                    break;
                }
                else
                {
                    whitenum++;
                }
            }
        }
        /*************找到左下或右下拐点后，拟合并预测中线，然后再顺着预测后的中线找**************/
        if (findrightdownguai == 1 || findleftdownguai == 1)
        {

            if (findrightdownguai == 1 && findleftdownguai == 0) // 左斜入十字，仅有右下拐点，取右下拐点下的中线行
            {
                if (!three_cross && !three_cross1 && !youhuihuan_flag && !lefthuihuan_flag) // 防三叉
                {
                    for (unsigned int j = 68; j >= 1; j--)
                    {
                        // 左上拐点
                        if (((j < (unsigned int)left_turn_down[0]) && ((left_line[j] - left_line[j + 3]) >= 10) && ((left_line[j] - left_line[j + 2]) >= 10) && ((left_line[j] - left_line[j + 1]) >= 10)) && Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0)
                        {

                            left_turn_up[0] = j - 2; // 数组里面没有第0行
                            left_turn_up[1] = left_line[j] - 2;
                            // 获得的上坐标先确定一下是不是比下坐标小，如果小则说明提前断掉，此时的“上拐点”为假.
                            // 如果比下坐标大则此时的“上拐点”为真.
                            if (left_turn_up[0] >= left_turn_down[0])
                            {
                                ;
                            }
                            else
                                break;
                        }
                    }
                    /***找右上拐点***********/
                    for (unsigned int j = 68; j >= 1; j--)
                    {
                        if (((j < (unsigned int)right_turn_down[0]) && right_line[j + 3] - right_line[j] >= 10 && right_line[j + 2] - right_line[j] >= 10 && right_line[j + 1] - right_line[j] >= 10) && (Right_Add[j] == 0 && Right_Add[j - 1] == 0 && Right_Add[j - 2] == 0))
                        {
                            right_turn_up[0] = j - 2;
                            right_turn_up[1] = right_line[j - 2];
                            if (right_turn_up[0] >= right_turn_down[0])
                            {
                                ;
                            }
                            else
                                break;
                        }
                    }
                    if (right_turn_up[0] > (sousuojieshuhang) && right_turn_up[0] < right_turn_down[0] && right_turn_up[0] != 0)
                    {

                        findrightupguai = 1; // 表示找到右上拐点了
                    }
                    if (left_turn_up[0] > (sousuojieshuhang) && left_turn_up[0] < left_turn_down[0] && left_turn_up[0] != 0)
                    {

                        findleftupguai = 1; // 表示找到左上拐点了
                    }

                    /*********开始补线(找到左下拐点和左上拐点  或 找到右下拐点和右上拐点)*********/
                    if ((findrightupguai == 1 && findrightdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 0))
                    {

                        if (findleftupguai == 1 && findleftdownguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                        {
                            unsigned int start1 = left_turn_down[0] + 2;
                            if (start1 >= 68)
                                start1 = 68;

                            unsigned int start2 = left_turn_up[0];
                            if (start2 >= 68)
                                start2 = 68;

                            left_buxian(left_line[start1], start1, left_line[start2], start2);
                        }
                        else if (findleftupguai == 1 && findleftdownguai == 0)
                        {
                            unsigned int start1 = left_turn_up[0];
                            if (start1 >= 68)
                                start1 = 68;

                            left_buxian(2, 68, left_line[start1], start1);
                        }
                        // 并列关系
                        if (findrightdownguai == 1 && findrightupguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                        {
                            unsigned int start1 = right_turn_down[0] + 2;
                            if (start1 >= 68)
                                start1 = 68;

                            unsigned int start2 = right_turn_up[0];
                            if (start2 >= 68)
                                start2 = 68;
                            unsigned int end2 = right_turn_up[0];
                            right_buxian(right_line[start1], start1, right_line[end2], end2);
                        }
                        else if (findrightdownguai == 0 && findrightupguai == 1)
                        {
                            unsigned int start1 = right_turn_up[0];
                            if (start1 >= 68)
                                start1 = 68;

                            right_buxian(184, 68, right_line[start1], start1);
                        }
                    }
                    else if (!three_cross && right_turn_down[0] < 60) ////?
                        sousuojieshuhang = right_turn_down[0] + 1;
                }
            }
            else if (findrightdownguai == 0 && findleftdownguai == 1) // 右斜入十字，仅有左下拐点，取左下拐点下的中线行
            {
                if (!three_cross && !three_cross1 && !youhuihuan_flag && !lefthuihuan_flag)
                {
                    for (unsigned int j = 68; j >= 1; j--)
                    {
                        // 左上拐点
                        if (((j < (unsigned int)left_turn_down[0]) && ((left_line[j] - left_line[j + 3]) >= 10) && ((left_line[j] - left_line[j + 2]) >= 10) && ((left_line[j] - left_line[j + 1]) >= 10)) && Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0)
                        {
                            left_turn_up[0] = j - 2; // 数组里面没有第0行
                            left_turn_up[1] = left_line[j] - 2;
                            // 获得的上坐标先确定一下是不是比下坐标小，如果小则说明提前断掉，此时的“上拐点”为假.
                            // 如果比下坐标大则此时的“上拐点”为真.
                            if (left_turn_up[0] >= left_turn_down[0])
                            {
                                ;
                            }
                            else
                                break;
                        }
                    }
                    /***找右上拐点***********/
                    for (unsigned int j = 68; j >= 1; j--)
                    {
                        if (((j < (unsigned int)right_turn_down[0]) && right_line[j + 3] - right_line[j] >= 10 && right_line[j + 2] - right_line[j] >= 10 && right_line[j + 1] - right_line[j] >= 10) && (Right_Add[j] == 0 && Right_Add[j - 1] == 0 && Right_Add[j - 2] == 0))
                        {
                            right_turn_up[0] = j - 2;
                            right_turn_up[1] = right_line[j - 2];
                            if (right_turn_up[0] >= right_turn_down[0])
                            {
                                ;
                            }
                            else
                                break;
                        }
                    }
                    if (right_turn_up[0] > (sousuojieshuhang) && right_turn_up[0] < right_turn_down[0] && right_turn_up[0] != 0)
                    {

                        findrightupguai = 1; // 表示找到右上拐点了
                    }
                    if (left_turn_up[0] > (sousuojieshuhang) && left_turn_up[0] < left_turn_down[0] && left_turn_up[0] != 0)
                    {

                        findleftupguai = 1; // 表示找到左上拐点了
                    }

                    /*********开始补线(找到左下拐点和左上拐点  或 找到右下拐点和右上拐点)*********/
                    if ((findleftupguai == 1 && findleftdownguai == 1) || (findleftupguai == 1 && findleftdownguai == 0))
                    {

                        if (findleftupguai == 1 && findleftdownguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                        {
                            unsigned int start1 = left_turn_down[0] + 2;
                            if (start1 >= 68)
                                start1 = 68;

                            unsigned int start2 = left_turn_up[0];
                            if (start2 >= 68)
                                start2 = 68;

                            left_buxian(left_line[start1], start1, left_line[start2], start2);
                        }
                        else if (findleftupguai == 1 && findleftdownguai == 0)
                        {
                            unsigned int start1 = left_turn_up[0];
                            if (start1 >= 68)
                                start1 = 68;

                            left_buxian(2, 68, left_line[start1], start1);
                        }
                        // 并列关系
                        if (findrightdownguai == 1 && findrightupguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                        {
                            unsigned int start1 = right_turn_down[0] + 2;
                            if (start1 >= 68)
                                start1 = 68;

                            unsigned int start2 = right_turn_up[0];
                            if (start2 >= 68)
                                start2 = 68;
                            unsigned int end2 = right_turn_up[0];
                            right_buxian(right_line[start1], start1, right_line[end2], end2);
                        }
                        else if (findrightdownguai == 0 && findrightupguai == 1)
                        {
                            unsigned int start1 = right_turn_up[0];
                            if (start1 >= 68)
                                start1 = 68;

                            right_buxian(184, 68, right_line[start1], start1);
                        }
                    }
                    else if (!three_cross && left_turn_down[0] < 60)
                        sousuojieshuhang = left_turn_down[0] + 1;
                }
            }
            else if (findrightdownguai == 1 && findleftdownguai == 1) // 正入十字：用两个下拐点中最小行下的中线行，拟合出k，b，进而拟合出预测中线
            {
                ////std::cout << "\033[32m找到双拐点\033[0m" << std::endl;

                xielv_1eft2 = abs(left_line[left_turn_down[0] + 5] - left_turn_down[1]) + abs(left_line[left_turn_down[0] - 5] - left_turn_down[1]);
                xielv_right2 = abs(right_line[right_turn_down[0] + 5] - right_turn_down[1]) + abs(right_line[right_turn_down[0] - 5] - right_turn_down[1]);
                if (
                    whitenum != 0 && whitenum < 54 &&
                    Left_Add_num2 == 0 && Right_Add_num2 == 0 &&
                    (!lefthuihuan_flag && !youhuihuan_flag) && ((left_turn_down[0] > 18 && left_turn_down[0] != 69) || (right_turn_down[0] > 18 && right_turn_down[0] != 69)) &&
                    abs(left_turn_down[0] - right_turn_down[0]) < 30 &&
                    ((xielv_1eft2 < 30 && xielv_right2 < 30) || (xielv_1eft2 < 20 && xielv_right2 > 30) || (xielv_1eft2 > 20 && xielv_right2 < 30)) && xielv_1eft2 < 50 && xielv_right2 < 50)
                {
                    xj = left_turn_down[1];  //||((xielv_1eft2<20&& xielv_right2>30))
                    xk = right_turn_down[1]; //||((xielv_1eft2>30&& xielv_right2<20))
                    xj = range_protect(xj, 2, 184);
                    xk = range_protect(xk, 2, 184);

                    for (unsigned int j = xj + 10; j <= xk - 10; j++)
                    {
                        if (white_num_col[j] < white_num_col_min)
                        {
                            white_num_col_min = white_num_col[j];
                            white_num_col_min_line = j;
                        }
                    }

                    if (white_num_col_min_line <= (th_y + 15) && white_num_col_min_line >= (th_y - 15)) // 10
                    {
                        if (white_num_col_line > (th_y + 15) || white_num_col_line < (th_y - 15))
                        {
                            cnt3++;
                        }
                        if (cnt3 > 1)
                        {
                            cnt3 = 0;
                        }
                    }
                }

                if (!three_cross && !three_cross1 && !youhuihuan_flag && !lefthuihuan_flag)
                {
                    ////std::cout<<"进入第二回环"<<std::endl;
                    for (unsigned int j = 65; j >= 3; j--)
                    {
                        ////std::cout << "\033[32m左上拐点检索\033[0m" << std::endl;
                        // 左上拐点
                        if (((j < (unsigned int)left_turn_down[0]) && ((left_line[j] - left_line[j + 3]) >= 10) && ((left_line[j] - left_line[j + 2]) >= 10) && ((left_line[j] - left_line[j + 1]) >= 10)) && Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0)
                        {

                            left_turn_up[0] = j - 3; // 数组里面没有第0行
                            left_turn_up[1] = left_line[j] - 3;
                            // 获得的上坐标先确定一下是不是比下坐标小，如果小则说明提前断掉，此时的“上拐点”为假.
                            // 如果比下坐标大则此时的“上拐点”为真.
                            if (left_turn_up[0] >= left_turn_down[0])
                            {
                                ////std::cout << "\033[32m找到左上拐点\033[0m" << std::endl;
                            }
                            else
                            {
                                ////std::cout << "\033[32m找左上拐点失败\033[0m" << std::endl;
                                break;
                            }
                        }
                    }
                    /***找右上拐点***********/
                    for (unsigned int j = 65; j >= 3; j--)
                    {
                        ////std::cout << "\033[32m右上拐点检索\033[0m" << std::endl;
                        if (((j < (unsigned int)right_turn_down[0]) && right_line[j + 3] - right_line[j] >= 10 && right_line[j + 2] - right_line[j] >= 10 && right_line[j + 1] - right_line[j] >= 10) && (Right_Add[j] == 0 && Right_Add[j - 1] == 0 && Right_Add[j - 2] == 0))
                        {
                            right_turn_up[0] = j - 3;
                            right_turn_up[1] = right_line[j - 3];
                            if (right_turn_up[0] >= right_turn_down[0])
                            {
                                ////std::cout << "\033[32m找到右上拐点\033[0m" << std::endl;
                            }
                            else
                            {
                                ////std::cout << "\033[32m找右上拐点失败\033[0m" << std::endl;
                                break;
                            }
                        }
                    }
                    if (right_turn_up[0] > (sousuojieshuhang) && right_turn_up[0] < right_turn_down[0] && right_turn_up[0] != 0)
                    {
                        ////std::cout << "\033[31m这是红色文字\033[0m" << std::endl;
                        ////std::cout << "\033[32m找到右上拐点\033[0m" << std::endl;
                        findrightupguai = 1; // 表示找到右上拐点了
                    }
                    if (left_turn_up[0] > (sousuojieshuhang) && left_turn_up[0] < left_turn_down[0] && left_turn_up[0] != 0)
                    {
                        ////std::cout << "\033[32m找到左上拐点\033[0m" << std::endl;
                        findleftupguai = 1; // 表示找到左上拐点了
                    }

                    /*********开始补线(找到左下拐点和左上拐点  或 找到右下拐点和右上拐点)*********/
                    if ((findleftupguai == 1 && findleftdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 0) || (findleftupguai == 1 && findleftdownguai == 0))
                    {
                        ////std::cout << "\033[32m开始补线\033[0m" << std::endl;
                        if (findleftupguai == 1 && findleftdownguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                        {
                            unsigned int start1 = left_turn_down[0] + 2;
                            if (start1 >= 68)
                                start1 = 68;

                            unsigned int start2 = left_turn_up[0];
                            if (start2 >= 68)
                                start2 = 68;

                            left_buxian(left_line[start1], start1, left_line[start2], start2);
                        }
                        else if (findleftupguai == 1 && findleftdownguai == 0)
                        {
                            unsigned int start1 = left_turn_up[0];
                            if (start1 >= 68)
                                start1 = 68;

                            left_buxian(2, 68, left_line[start1], start1);
                        }
                        // 并列关系
                        if (findrightdownguai == 1 && findrightupguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                        {
                            unsigned int start1 = right_turn_down[0] + 2;
                            if (start1 >= 68)
                                start1 = 68;

                            unsigned int start2 = right_turn_up[0];
                            if (start2 >= 68)
                                start2 = 68;
                            unsigned int end2 = right_turn_up[0];
                            right_buxian(right_line[start1], start1, right_line[end2], end2);
                        }
                        else if (findrightdownguai == 0 && findrightupguai == 1)
                        {
                            int start1 = right_turn_up[0];
                            if (start1 >= 68)
                                start1 = 68;

                            right_buxian(184, 68, right_line[start1], start1);
                        }
                    }
                }
            }
        }
        if (!youhuihuan_flag && !lefthuihuan_flag && !three_cross1 && !three_cross && (!left_huan_num && !right_huan_num) && (l_start < 65 && r_start < 65 && abs(l_start - r_start) <= 12) && times >= 5)
        {
            for (unsigned int j = 68; j >= 1; j--)
            {
                // 左上拐点
                if (((j < (unsigned int)left_turn_down[0]) && ((left_line[j] - left_line[j + 3]) >= 10) && ((left_line[j] - left_line[j + 2]) >= 10) && ((left_line[j] - left_line[j + 1]) >= 10)) && Left_Add[j] == 0 && Left_Add[j - 1] == 0 && Left_Add[j - 2] == 0)
                {

                    left_turn_up[0] = j - 2; // 数组里面没有第0行
                    left_turn_up[1] = left_line[j] - 2;
                    // 获得的上坐标先确定一下是不是比下坐标小，如果小则说明提前断掉，此时的“上拐点”为假.
                    // 如果比下坐标大则此时的“上拐点”为真.
                    if (left_turn_up[0] >= left_turn_down[0])
                    {
                        ;
                    }
                    else
                        break;
                }
            }
            /***找右上拐点***********/
            for (unsigned int j = 68; j >= 1; j--)
            {
                if (((j < (unsigned int)right_turn_down[0]) && right_line[j + 3] - right_line[j] >= 20 && right_line[j + 2] - right_line[j] >= 20 && right_line[j + 1] - right_line[j] >= 20) && (Right_Add[j] == 0 && Right_Add[j - 1] == 0 && Right_Add[j - 2] == 0))
                {
                    right_turn_up[0] = j - 2;
                    right_turn_up[1] = right_line[j - 2];
                    if (right_turn_up[0] >= right_turn_down[0])
                    {
                        ;
                    }
                    else
                        break;
                }
            }
            if (right_turn_up[0] > (sousuojieshuhang) && right_turn_up[0] < right_turn_down[0] && right_turn_up[1] <= right_turn_down[1] && right_turn_up[0] != 0)
            {

                findrightupguai = 1; // 表示找到右上拐点了
            }
            if (left_turn_up[0] > (sousuojieshuhang) && left_turn_up[0] < left_turn_down[0] && left_turn_up[1] >= left_turn_down[1] && left_turn_up[0] != 0)
            {

                findleftupguai = 1; // 表示找到左上拐点了
            }
            /*********开始补线(找到左下拐点和左上拐点  或 找到右下拐点和右上拐点)*********/
            if ((findleftupguai == 1 && findleftdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 1) || (findrightupguai == 1 && findrightdownguai == 0) || (findleftupguai == 1 && findleftdownguai == 0))
            {

                if (findleftupguai == 1 && findleftdownguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                {
                    unsigned int start1 = left_turn_down[0] + 2;
                    if (start1 >= 68)
                        start1 = 68;

                    unsigned int start2 = left_turn_up[0];
                    if (start2 >= 68)
                        start2 = 68;

                    left_buxian(left_line[start1], start1, left_line[start2], start2);
                }
                else if (findleftupguai == 1 && findleftdownguai == 0)
                {
                    unsigned int start1 = left_turn_up[0];
                    if (start1 >= 68)
                        start1 = 68;

                    left_buxian(2, 68, left_line[start1], start1);
                }
                // 并列关系
                if (findrightdownguai == 1 && findrightupguai == 1) // 找到左下拐点和左上拐点,拟合所需的点是下拐点下面三个点和上拐点上面三个点
                {
                    unsigned int start1 = right_turn_down[0] + 2;
                    if (start1 >= 68)
                        start1 = 68;
                    unsigned int end1 = right_turn_down[0] + 1;
                    unsigned int start2 = right_turn_up[0];
                    if (start2 >= 68)
                        start2 = 68;
                    unsigned int end2 = right_turn_up[0];
                    right_buxian(right_line[start1], start1, right_line[end2], end2);
                }
                else if (findrightdownguai == 0 && findrightupguai == 1)
                {
                    unsigned int start1 = right_turn_up[0];
                    if (start1 >= 68)
                        start1 = 68;
                    unsigned int end1 = right_turn_up[0];
                    right_buxian(184, 68, right_line[start1], start1);
                }
            }
        }

        ////右环岛
        guaidian = 0;
        if (!youhuihuan_flag && !left_huan_num && !three_cross && !poer_flag && !star_lineflag && !por_cnt) //
        {
            find_rightup_point(10, 65);
            find_rightdown_point(65, 22, 2);
            right_turn_down[0] = 69;
            m = Right_Add_num - Left_Add_num;
            if (right_huan_num == 0 && !right_huan_num && (m > 11) && sousuojieshuhang <= 9 && l_start >= 65 && r_start >= 65 && (Right_Add_num >= 17) && (Left_Add_num <= 3) && (zuodiuxianshu < 3) && (youdiuxianshu > 17) && k_left < -0.6 && k_left > -1.3)
            {
                youhuandao_flag = 1;
                right_huan_num = 1;

                time3 = 0;
            }
            else if (right_huan_num == 1 && l_start >= 65 && r_start <= 40) //&&(ad_guiyi[2]>100||ad_guiyi[0]>100)
            {
                right_huan_num = 2;
                huan2_flag = 1;
            }
            else if (right_huan_num == 2)
            {
                find_rightmiddle_point(50, 20);
                guaidian = right_turn_middle[0];
                if (huan2_flag == 0)
                    right_huan_num = 0;
                if (right_huan_num == 2 && r_start >= 67 && guaidian >= 22) // l_start>= 50&&r_start<=35
                {
                    cnt2++; // 连续3次判定
                }
                else
                    cnt2 = 0;

                if (cnt2 >= 2 && gou_Guo2 == 0)
                {

                    right_huan_num = 3;

                    cnt2 = 0;
                }
            }
            else if (right_huan_num == 3 && right_turn_up[0] > 40) //>35)//right_turn_up[0]////right_huan_num==3&&r_start<=40
            {
                guaidian = 0;
                right_huan_num = 5; // 更换搜线方式
            }
            else if (right_huan_num == 5 && white_num_col_line > 160)
            {
                right_huan_num = 6;
            }

            else if (right_huan_num == 6 && Left_Add_num > 8 && left_turn_down[0] > 25 && left_turn_down[0] != 69) //&&left_turn_down[0]<50
            {
                right_huan_num = 7;
            }
            else if (right_huan_num == 7 && sousuojieshuhang <= 15 && zuodiuxianshu < 2 && Left_Add_num < 3 && (huandao_7 >= 10 || (k_left < -0.6 && k_left > -1.3))) //&&zuodiuxianshu<5
            {
                right_huan_num = 8; //&&white_num_col[93]>50
            }
            else if (right_huan_num == 8 && r_start < 60)
            {
                right_huan_num = 9;
            }

            else if (right_huan_num == 9 && l_start > 60 && r_start > 60)
            {

                youhuandao_flag = 0;
                youhuandao_flag_queding = 0;
                youhuandao_flag2 = 0;
                right_huan_num = 0;
                time2 = 0;
                youhuandao_flag3 = 0;
            }
        }

        ////youhuandao_deal
        find_rightdown_point(60, 10, 2);
        find_rightmiddle_point(60, 8);
        find_rightup_point(5, 65);
        if (right_huan_num == 1)
        {

            // if(flag_find_huan_rightmiddle_point)
            //{
            //   right_buxian(right_turn_middle[1],right_turn_middle[0],160,right_turn_down[0]);
            //   flag_find_huan_rightmiddle_point=0;
            // }
            right_buxian(left_line[15] + 35, 15, 170, 68);
            //  sousuojieshuhang=15;
        }
        else if (right_huan_num == 2)
        {
            // if(flag_find_huan_rightmiddle_point)
            // {
            //  right_buxian(right_turn_middle[1],right_turn_middle[0],160,68);
            //   flag_find_huan_rightmiddle_point=0;
            // }
            right_buxian(left_line[15] + 35, 15, 170, 68);
            // sousuojieshuhang=15;
        }
        else if (right_huan_num == 3)
        {

            //      if(flag_find_huan_rightmiddle_point)
            //         {
            //          flag_find_huan_rightmiddle_point=0;
            //          if(right_turn_up[0]>=13)//21
            //             {
            //              right_buxian2(right_turn_up[1]+14,right_turn_up[0],left_line[right_turn_middle[0]],right_turn_middle[0]);
            //             for(int ql=right_turn_up[0];ql>=10;ql--)
            //                   {
            //                      Right_Line_New[ql]=180;
            //                      Left_Line_New[ql]=140;
            //                   }
            //             }
            //         }
            //      else
            //      {
            if (right_turn_up[0] >= 12) // 21
            {
                right_buxian2(right_turn_up[1] + 30, right_turn_up[0], left_line[68], 68);
                for (int ql = right_turn_up[0]; ql >= 10; ql--)
                {
                    Right_Line_New[ql] = 183;
                    Left_Line_New[ql] = 140;
                }
                sousuojieshuhang = 15;
            }
            //   }
        }
        else if (right_huan_num == 4)
        {
            for (int ql = 68; ql >= 17; ql--)
            {
                Right_Line_New[ql] = 184;
                Left_Line_New[ql] = 67;
            }
            for (y = 68; y > 5; y--) // x是减59—56  num是加0—4
            {
                for (unsigned int x = 160; x <= 184; x++) // 中间向右找跳变
                {
                    if (image_use[y][x - 1] == 255 && image_use[y][x] == 255 && image_use[y][x + 1] == 0 && image_use[y][x + 2] == 0) // 两个连续黑点触发
                    {

                        Right_Line_New[y] = x;
                        break;
                    }
                    else if (x == 184) // 到最右边了都没扫到黑点a
                    {

                        Right_Line_New[y] = x;
                        break;
                    }
                }

                for (unsigned int x = 160; x >= 1; x--) // 中间向左找跳变
                {
                    if (image_use[y][x - 1] == 0 && image_use[y][x] == 0 && image_use[y][x + 1] == 255 && image_use[y][x + 2] == 255) // 两个连续黑点触发
                    {
                        Left_Line_New[y] = x + 1;
                        break;
                    }
                    else if (x + 1 == 67) // 到最左边了都没扫到黑点
                    {
                        Left_Line_New[y] = x + 1;
                        break;
                    }
                }
                if (abs(Right_Line_New[y] - Left_Line_New[y]) < 3)
                    break;
                if (image_use[y][(Right_Line_New[y] + Left_Line_New[y]) / 2] == 0)
                    break;
                if (y <= 1)
                    break;
            }
            sousuojieshuhang = y + 1;
            left_buxian(right_turn_up[1], right_turn_up[0], 30, 68);
            ////入环补线
        }
        else if (right_huan_num == 5)
        {
            for (int ql = 68; ql >= 17; ql--)
            {
                Right_Line_New[ql] = 184;
                Left_Line_New[ql] = 67;
            }
            for (y = 68; y > 5; y--) // x是减59—56  num是加0—4
            {
                for (unsigned int x = 160; x <= 184; x++) // 中间向右找跳变
                {
                    if (image_use[y][x - 1] == 255 && image_use[y][x] == 255 && image_use[y][x + 1] == 0 && image_use[y][x + 2] == 0) // 两个连续黑点触发
                    {

                        Right_Line_New[y] = x;
                        break;
                    }
                    else if (x == 184) // 到最右边了都没扫到黑点a
                    {

                        Right_Line_New[y] = x;

                        break;
                    }
                }

                for (unsigned int x = 160; x >= 1; x--) // 中间向左找跳变
                {
                    if (image_use[y][x - 1] == 0 && image_use[y][x] == 0 && image_use[y][x + 1] == 255 && image_use[y][x + 2] == 255) // 两个连续黑点触发
                    {
                        Left_Line_New[y] = x + 1;
                        break;
                    }
                    else if (x + 1 == 67) // 到最左边了都没扫到黑点
                    {
                        Left_Line_New[y] = x + 1;
                        break;
                    }
                }
                if (abs(Right_Line_New[y] - Left_Line_New[y]) < 3)
                    break;
                if (image_use[y][(Right_Line_New[y] + Left_Line_New[y]) / 2] == 0)
                    break;
                if (y <= 1)
                    break;
            }
            sousuojieshuhang = y + 1;
            left_buxian(right_turn_up[1] + 18, right_turn_up[0], 30, 68);
        }
        else if (right_huan_num == 7)
        {
            if (!(zuodiuxianshu < 15 && Left_Add_num < 3))
            {
                find_leftdown_point(65, 25, 1); // 1是十字
                right_buxian2(162, sousuojieshuhang, left_turn_down[1] + 20, left_turn_down[0]);
                ////出环补线
            }
        }

        else if (right_huan_num == 8)
        {

            right_buxian(left_line[15] + 55, 15, 170, 68);
            ////直道补线
        }

        ////修复中线
        int count = sousuojieshuhang;
        for (unsigned int i = 68; i > count; i--)
        {
            //// 赛道宽度动态调整
            Half_width_handao[i] = Half_width_handao2[i] + huan_bankuan;
            Half_width[i] = Half_width2[i] + com_bankuan;
            {
                ////左边界贴左边缘（≤18）且右边界未贴右边缘（<184）
                if (Left_Line_New[i] <= 18 && Right_Line_New[i] < 184 && !three_cross && right_huan_num != 7 && left_huan_num != 7 && right_huan_num != 5 && left_huan_num != 5 && right_huan_num != 3 && left_huan_num != 3)
                {
                    if (Right_Line_New[i] - Half_width[i] <= 18)
                        center[i] = 18;
                    else if (right_huan_num == 8 || left_huan_num == 8 ||
                             right_huan_num == 9 || left_huan_num == 9 ||
                             right_huan_num == 1 || left_huan_num == 1 ||
                             right_huan_num == 2 || left_huan_num == 2 ||
                             youhuihuan_flag == 1 || lefthuihuan_flag == 1 || (huihuan_num > 18 && !poer_flag))
                    {
                        center[i] = Right_Line_New[i] - Half_width_yuanshi[i]; //||huihuan_num>15||sousuojieshuhang<=3
                        // std::cout<<"循环1"<<std::endl;
                    }
                    else
                    {
                        if (right_huan_num != 0 || left_huan_num != 0)
                            center[i] = Right_Line_New[i] - Half_width_handao[i];
                        else
                            center[i] = Right_Line_New[i] - Half_width[i];
                        // std::cout<<"循环2"<<std::endl;
                    }
                }
                ////右边界贴右边缘（≥184）且左边界未贴左边缘（>18）
                else if (Left_Line_New[i] > 18 && Right_Line_New[i] >= 184 && !three_cross && right_huan_num != 7 && left_huan_num != 7 && right_huan_num != 5 && left_huan_num != 5 && right_huan_num != 3 && left_huan_num != 3)
                {
                    if (Left_Line_New[i] + Half_width[i] >= 184)
                        center[i] = 184;
                    else if (
                        right_huan_num == 8 || left_huan_num == 8 ||
                        right_huan_num == 9 || left_huan_num == 9 ||
                        right_huan_num == 1 || left_huan_num == 1 ||
                        right_huan_num == 2 || left_huan_num == 2 ||
                        youhuihuan_flag == 1 || lefthuihuan_flag == 1 || (huihuan_num > 18 && !poer_flag))
                        center[i] = Left_Line_New[i] + Half_width_yuanshi[i];
                    else
                    {
                        if (right_huan_num != 0 || left_huan_num != 0)
                            center[i] = Left_Line_New[i] + Half_width_handao[i];
                        else
                            center[i] = Left_Line_New[i] + Half_width[i];
                    }
                }
                ////常规情况（左右边界均有效）
                else
                    center[i] = (Left_Line_New[i] + Right_Line_New[i]) / 2; // 左右边界都是修复过的,得到中线
            }
            ////保护机制 限制中线在图像有效范围内
            center[i] = range_protect(center[i], 18, 184); // 限幅保护
        }

        /******************************************************************曲率计算**************************************************************/
        ////近端曲率统计（60行到count行）   统计中线方向变化次数，差值越大说明弯道越急
        for (unsigned int i = 60; i > count; i--)
        {

            if ((center[y] - center[y + 1]) > 0)
                qulv_jinduan_right++;
            else if ((center[y] - center[y + 1]) < 0)
                qulv_jinduan_left++;
        }
        //  qvlv_quanju=abs(qvlv_quanju_right-qvlv_quanju_left);//曲率全局qvlv_quanju qulv_jinduan qulv_yuandaun
        qulv_jinduan = abs(qulv_jinduan_right - qulv_jinduan_left); // 曲率近端
        //  qulv_yuandaun=abs(qulv_yuandaun_right-qulv_yuandaun_left);//曲率远端

        //// 中线斜率拟合
        regression(0, sousuojieshuhang + 3, 65);
        k_center = parameterB;
        for (unsigned int i = 68; i > sousuojieshuhang; i--)
        {
            //         image_use[i][left[i]+3] =1;
            //          image_use[i][right[i]-3] =2;
            image_use[i][center[i]] = 0;
            image_use[i][Left_Line_New[i] + 3] = 0;
            image_use[i][Right_Line_New[i] - 3] = 0;
            //     image_use[i][center_th[i]] =0;
        }

        cv::Mat haha;

        for (int iqq = 68; iqq > sousuojieshuhang; iqq--)
        {
            cv::circle(get_color, cv::Point(left_line[iqq] + 2, iqq), 1, cv::Scalar(0, 0, 150), -1);      // -1 表示实心圆
            cv::circle(get_color, cv::Point(right_line[iqq] - 2, iqq), 1, cv::Scalar(0, 0, 150), -1);     // -1 表示实心圆
            cv::circle(get_color, cv::Point(Left_Line_New[iqq] + 2, iqq), 1, cv::Scalar(0, 0, 255), -1);  // -1 表示实心圆
            cv::circle(get_color, cv::Point(Right_Line_New[iqq] - 2, iqq), 1, cv::Scalar(0, 0, 255), -1); // -1 表示实心圆

            // cv::circle(get_color, cv::Point(center_th[iqq]-2, iqq), 1, cv::Scalar(0,0,255), -1);  // -1 表示实心圆
            cv::circle(get_color, cv::Point(center[iqq] - 2, iqq), 1, cv::Scalar(0, 255, 0), -1); // -1 表示实心圆
        }
        for (y = 68; y > 20; y--)
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

        cv::resize(get_color, haha, cv::Size(188 * 5, 70 * 5));
        cv::putText(haha, std::to_string(white_num_col_line), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

        cv::putText(haha, std::to_string(findleftdownguai), cv::Point(10, 40), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);
        cv::putText(haha, std::to_string(findrightdownguai), cv::Point(30, 40), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

        cv::putText(haha, std::to_string(k_center), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

        cv::putText(haha, std::to_string(right_huan_num), cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

        cv::putText(haha, std::to_string(Left_Add_num), cv::Point(10, 100), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);
        cv::putText(haha, std::to_string(left_turn_down[0]), cv::Point(30, 100), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

        {
            std::lock_guard<std::mutex> lock(image_mutex); // 锁住图像数据，确保线程安全
            image_to_send = haha.clone();                  // 拷贝图像数据
            // image_to_send = canvas.clone(); // 拷贝图像数据
        }

        // std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
