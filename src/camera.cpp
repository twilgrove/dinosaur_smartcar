
#include "camera.h"
using namespace cv;

std::mutex CameraCapture_Mutex; // 摄像头采集资源互斥锁
std::mutex CameraCapture_Mutex_2; // 摄像头采集资源互斥锁
Img_Store Img_Store_c;
Img_Store* Img_Store_pp = &Img_Store_c;
/*
	摄像头初始化
		@参数说明
		Camera 摄像头
		Camera_EN 摄像头类型
		FPS 帧率
*/
bool CameraInit(cv::VideoCapture &Camera, int Camera_EN, int FPS)
{
	//相机类型设置
	switch (Camera_EN)
	{
        case 1:
        {
            //Camera.open("C:/Users/azw20/Desktop/own_smart_car/own_car/recorded/common.mp4"); // 配置路径
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
			std::cerr << "\033[31mimage_progress打开摄像头失败!!!\033[0m" << std::endl;
		}
		else{
			std::cerr << "\033[32mimage_progress成功打开摄像头!\033[0m" << std::endl;
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

void CameraImgGet(Img_Store* Img_Store_m,int runnings)
{
	while (Img_Store_m->Img_Capture.empty())
	{
        std::cout<<"image is empty!!!"<<std::endl;
		if (!runnings) ;
            //std::cout<<"图像为空且running为0"<<std::endl;;
	}

	CameraCapture_Mutex.lock();
	Img_Store_m->Img_Color = Img_Store_m->Img_Capture.front().clone();
	CameraCapture_Mutex.unlock();
}


void opencv_thread()
{
    while (running)
    {
        cv::Mat Img;//原图  无畸变图
        Camera >> Img; // 将视频流转为图像流
		// 缩放比例
    	double scale = 188.0 / Img.cols;
    	int new_height = static_cast<int>(Img.rows * scale);
		cv::Mat get_resized;
		cv::resize(Img, get_resized, cv::Size(188, new_height));
		if (new_height < 70) {
			std::cerr << "缩放后高度不足 70 像素，无法裁剪\n";
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
        //std::cout<<"存入图像..."<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
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
	//std::cout<<"预处理"<<std::endl;
	if (!(Img_Store_p->Img_Color).empty())
		cvtColor((Img_Store_p->Img_Color), (Img_Store_p->Img_Gray), COLOR_BGR2GRAY); // 彩色图像灰度化
	else 
		std::cerr << "\033[31mcvtColor 预处理第一步输入图像为空！\033[0m" << std::endl;
	// blur((Img_Store_p -> Img_Gray) , (Img_Store_p -> Img_Gray) , Size(18,18) , Point(-1,-1));	// 均值滤波
	threshold((Img_Store_p->Img_Gray), (Img_Store_p->Img_OTSU), 0, 255, THRESH_BINARY | THRESH_OTSU); // 灰度图像二值化
	// ImgSobel((Img_Store_p->Img_OTSU));													  // Sobel算子处理
	// threshold((Img_Store_p->Img_OTSU), (Img_Store_p->Img_OTSU), 0, 255, THRESH_BINARY | THRESH_OTSU); // 灰度图像二值化

	// ImgProcess::ImgSharpen((Img_Store_p -> Img_OTSU),5);
	for (int i = 0; i <= 1; i++)
	{
		dilate((Img_Store_p->Img_OTSU), (Img_Store_p->Img_OTSU), (Img_Store_p->Dilate_Kernel));
	}
	for (int i = 0; i <= 1; i++)
	{
		erode((Img_Store_p->Img_OTSU), (Img_Store_p->Img_OTSU), (Img_Store_p->Erode_Kernel));
	}
}