#ifndef _CAMERA_H_
#define _CAMERA_H_

struct Img_Store
{
    std::queue<cv::Mat> Img_Capture;                                                // 图像,类型：矩阵队列
    cv::Mat Img_Color;                                                              // 彩色图像
    cv::Mat Img_Color_Unpivot;                                                      // 彩色图像未旋转
    cv::Mat Img_Gray;                                                               // 灰度图像
    cv::Mat Img_Gray_Unpivot;                                                       // 灰度图像未旋转
    cv::Mat Img_OTSU;                                                               // 大津法二值化图像
    cv::Mat Img_OTSU_Unpivot;                                                       // 大津法二值化图像未旋转
    cv::Mat Img_Track;                                                              // 赛道识别图像
    cv::Mat Img_Track_Unpivot;                                                      // 赛道识别图像未旋转
    cv::Mat Img_Text;                                                               // 文本识别图像
    cv::Mat Img_All;                                                                // 所有图像
    cv::Mat Dilate_Kernel = getStructuringElement(cv::MORPH_CROSS, cv::Size(2, 2)); // 边线形态学膨胀核大小
    cv::Mat Erode_Kernel = getStructuringElement(cv::MORPH_CROSS, cv::Size(2, 2));  // 边线形态学腐蚀核大小
    int ImgNum;
};
extern Img_Store Img_Store_c;
extern Img_Store *Img_Store_pp; // 图像存储与指针
using namespace cv;

bool CameraInit(VideoCapture &Camera, int Camera_EN, int FPS);
void CameraImgGet(Img_Store *Img_Store_m, int runnings);
void opencv_thread();
void ImgSobel(Mat &Img);
void ImgPrepare(Img_Store *Img_Store_p);

#endif