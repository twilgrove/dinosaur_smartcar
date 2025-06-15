

#include "headfile.h"
using namespace cv;

std::mutex CameraCapture_Mutex;	  // 摄像头采集资源互斥锁
std::mutex CameraCapture_Mutex_2; // 摄像头采集资源互斥锁
Img_Store Img_Store_c;
Img_Store *Img_Store_pp = &Img_Store_c;
/*
	摄像头初始化
		@参数说明
		Camera 摄像头
		Camera_EN 摄像头类型
		FPS 帧率
*/
bool CameraInit(cv::VideoCapture &Camera, int Camera_EN, int FPS)
{
	// 相机类型设置
	switch (Camera_EN)
	{
	case 1:
	{
		// Camera.open("C:/Users/azw20/Desktop/own_smart_car/own_car/recorded/common.mp4"); // 配置路径
		Camera.open("C:/Users/azw20/Desktop/own_smart_car/own_car/recorded/output.mp4"); // 配置路径
		break;
	} // 演示视频
	case 2:
	{
		Camera.open("/dev/video0", CAP_V4L2);
		break;
	} // 摄像头video0
		if (!Camera.isOpened())
		{
			std::cerr << "\033[31mimage_progress open camera failed!!!\033[0m" << std::endl;
		}
		else
		{
			std::cerr << "\033[32mimage_progress open camera success!\033[0m" << std::endl;
		}
	}

	Camera.set(CAP_PROP_FRAME_WIDTH, 160);								  // 帧宽
	Camera.set(CAP_PROP_FRAME_HEIGHT, 120);								  // 帧高
	Camera.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G')); // 视频流格式
	Camera.set(CAP_PROP_FPS, FPS);										  // 帧率

	if (!Camera.isOpened())
	{
		std::cout << "\33[31mCamera:\33[0m init \33[31mfailed\33[0m" << std::endl;
		return false;
	}
	else
	{
		std::cout << "\33[34mCamera:\33[0m init success" << std::endl;
		return true;
	}
}

void CameraImgGet(Img_Store *Img_Store_m)
{
	CameraCapture_Mutex.lock();
	Img_Store_m->Img_Color = Img_Store_m->Img_Capture.front().clone();
	CameraCapture_Mutex.unlock();
}

void ImgSobel(Mat &Img)
{
	Mat ImgX;
	Mat ImgY;

	// 对X方向微分
	Sobel(Img, ImgX, CV_16S, 1, 0, 5); // x方向差分阶数 y方向差分阶数 核大小
	convertScaleAbs(ImgX, ImgX);	   // 可将任意类型的数据转化为CV_8UC1
	// 对Y方向微分
	Sobel(Img, ImgY, CV_16S, 0, 1, 5);		   // x方向差分阶数 y方向差分阶数 核大小
	convertScaleAbs(ImgY, ImgY);			   // 将任意类型的图像转化为CV_8UC1
	addWeighted(ImgX, 0.5, ImgY, 0.5, 0, Img); // 图像的线性混合
}
void ImgPrepare(Img_Store *Img_Store_p)
{
	(Img_Store_p->Img_Track) = (Img_Store_p->Img_Color).clone();
	// std::cout<<"预处理"<<std::endl;
	if (!(Img_Store_p->Img_Color).empty())
		cvtColor((Img_Store_p->Img_Color), (Img_Store_p->Img_Gray), COLOR_BGR2GRAY); // 彩色图像灰度化
	else
		std::cerr << "\033[31mcvtColor 预处理第一步输入图像为空！\033[0m" << std::endl;
	// blur((Img_Store_p -> Img_Gray) , (Img_Store_p -> Img_Gray) , Size(18,18) , Point(-1,-1));	// 均值滤波
	threshold((Img_Store_p->Img_Gray), (Img_Store_p->Img_OTSU), 0, 255, THRESH_BINARY | THRESH_OTSU); // 灰度图像二值化
	// ImgSobel((Img_Store_p->Img_OTSU));													  // Sobel算子处理
	// threshold((Img_Store_p->Img_OTSU), (Img_Store_p->Img_OTSU), 0, 255, THRESH_BINARY | THRESH_OTSU); // 灰度图像二值化

	// ImgProcess::ImgSharpen((Img_Store_p -> Img_OTSU),5);
	// for (int i = 0; i <= 1; i++)
	// {
	// 	dilate((Img_Store_p->Img_OTSU), (Img_Store_p->Img_OTSU), (Img_Store_p->Dilate_Kernel));
	// }
	for (int i = 0; i <= 1; i++)
	{
		erode((Img_Store_p->Img_OTSU), (Img_Store_p->Img_OTSU), (Img_Store_p->Erode_Kernel));
	}
}

void img_process()
{

	CameraImgGet(Img_Store_pp);
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
		cv::circle(get_color, cv::Point(left_line[iqq] + 2, iqq), 1, cv::Scalar(0, 0, 150), -1);		// -1 表示实心圆
		cv::circle(get_color, cv::Point(right_line[iqq] - 2, iqq), 1, cv::Scalar(0, 0, 150), -1);		// -1 表示实心圆
		cv::circle(get_color, cv::Point(Left_Line_New[iqq] + 2, iqq), 1, cv::Scalar(0, 255, 255), -1);	// -1 表示实心圆
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

	// cv::putText(haha, std::to_string(left_huan_num), cv::Point(10, 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 200, 0), 1, cv::LINE_AA);

	// cv::putText(haha, std::to_string(Left_Add_num), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);
	// cv::putText(haha, std::to_string(Right_Add_num), cv::Point(30, 50), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);

	// cv::putText(haha, std::to_string(zuodiuxianshu), cv::Point(30, 70), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);
	//  //float angles=-(Get_Point-87)*((1700000-1346000)/100)+(1700000+1346000)/2;
	//  int last_sp_duty;
	//  sp_duty=(int)(angles*0.6)+(int)(last_sp_duty*0.4);
	//  last_sp_duty=sp_duty;
	//  cv::putText(haha, std:.:to_string(sp_duty), cv::Point(30, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 0), 2, cv::LINE_AA);
	{
		image_mutex.lock();			  // 锁住图像数据，确保线程安全
		image_to_send = haha.clone(); // 拷贝图像数据

		cv::resize(Img_Store_pp->Img_OTSU, image_to_show, cv::Size(240, 90));
		for (int y = 0; y < 90; ++y)
		{
			for (int x = 0; x < 240; ++x)
			{
				image_show[y][x] = image_to_show.at<uchar>(y, x); // 值为 0 或 255
			}
		}
		image_mutex.unlock();
		// image_to_send = canvas.clone(); // 拷贝图像数据
	}
}

void opencv_thread()
{
	while (car.program_running)
	{
		// Test_period("opencv");

		cv::Mat Img;   // 原图  无畸变图
		Camera >> Img; // 将视频流转为图像流
		// 缩放比例
		if (!Img.empty())
		{
			double scale = 188.0 / Img.cols;
			int new_height = static_cast<int>(Img.rows * scale);
			cv::Mat get_resized;
			cv::resize(Img, get_resized, cv::Size(188, new_height));
			if (new_height < 70)
			{
				std::cerr << "new_height no 70,can't process\n";
			}
			cv::Rect roi(0, 20, 188, new_height - 20 - 51); // x, y, width, height
			cv::Mat Final_Img;
			cv::flip(get_resized(roi), Final_Img, -1);

			CameraCapture_Mutex.lock();
			if (!Img_Store_pp->Img_Capture.empty())
			{
				(Img_Store_pp->Img_Capture).pop();
			}
			(Img_Store_pp->Img_Capture).push(Final_Img);
			CameraCapture_Mutex.unlock();
		}
		if (!Img_Store_pp->Img_Capture.empty())
		{
			img_process();
			car.cam_frame++;
		}
		else
			std::cout << "image is empty!!!" << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(OPENCV_THREAD_PERIOD));
	}
}