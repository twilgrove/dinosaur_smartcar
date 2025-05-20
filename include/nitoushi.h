#ifndef NITOUSHI_H
#define NITOUSHI_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include "data_process.h"
#include "image_process.h"
extern int nitoushi_error;
// 从左右边线拐点中提取逆时针排序的四个矩形角点
inline std::vector<cv::Point2f> GetRectanglePointsFromInflection(Data_Path* Data_Path_p)
{
    std::vector<cv::Point2f> result;
    nitoushi_error=0;
    // 至少要两个左边线拐点和两个右边线拐点
    if (Data_Path_p->InflectionPointNum[0] < 2 || Data_Path_p->InflectionPointNum[1] < 2)
    {
        std::cerr << "L: " << Data_Path_p->InflectionPointNum[0]<< "  R: " << Data_Path_p->InflectionPointNum[1]<< "  Insufficient corner points to determine quadrilateral vertices." << std::endl;
        nitoushi_error=1;
        return result;
    }

    //左下
    float x0 = static_cast<float>(Data_Path_p->InflectionPointCoordinate[0][0]);
    float y0 = static_cast<float>(Data_Path_p->InflectionPointCoordinate[0][1]);
    // 右下
    float x1 = static_cast<float>(Data_Path_p->InflectionPointCoordinate[0][2]);
    float y1 = static_cast<float>(Data_Path_p->InflectionPointCoordinate[0][3]);

    // 右上
    float x2 = static_cast<float>(Data_Path_p->InflectionPointCoordinate[1][2]);
    float y2 = static_cast<float>(Data_Path_p->InflectionPointCoordinate[1][3]);
    // 左上
    float x3 = static_cast<float>(Data_Path_p->InflectionPointCoordinate[1][0]);
    float y3 = static_cast<float>(Data_Path_p->InflectionPointCoordinate[1][1]);
    

    
    result.emplace_back(x0, y0); //
    result.emplace_back(x1, y1); //
    result.emplace_back(x2, y2); // 
    result.emplace_back(x3, y3); // 
    

    // 输出坐标
    int woaiwaluolante=0;
    for(woaiwaluolante = 0; woaiwaluolante < result.size(); ++woaiwaluolante)
    {
        std::cout << "Point " << woaiwaluolante << ": (" << result[woaiwaluolante].x << ", " << result[woaiwaluolante].y << ")" << std::endl;
    }
    

    return result;
}

inline int CheckPattern(Data_Path *Data_Path_p, int noiseThreshold = 3, int minDecreasingPoints = 5) {
    int startIndex = -1;

    for(int i=0;i<=Data_Path_p->NumSearch[1];i++)//会读取前5后5数据，所以前面对输入范围有要求
    {
        if(Data_Path_p->SideCoordinate[i][2]==Data_Path_p->SideCoordinate[i+4][2]&&Data_Path_p->SideCoordinate[i][2] <Data_Path_p->SideCoordinate[i-4][2]&&
        Data_Path_p->SideCoordinate[i][2]==Data_Path_p->SideCoordinate[i+3][2]&&Data_Path_p->SideCoordinate[i][2]==Data_Path_p->SideCoordinate[i-3][2]&&
        Data_Path_p->SideCoordinate[i][2]==Data_Path_p->SideCoordinate[i+2][2]&&Data_Path_p->SideCoordinate[i][2]==Data_Path_p->SideCoordinate[i-2][2]&&
        Data_Path_p->SideCoordinate[i][2]==Data_Path_p->SideCoordinate[i+1][2]&&Data_Path_p->SideCoordinate[i][2]==Data_Path_p->SideCoordinate[i-1][2])
        {//一堆数据一样，显然不能作为单调转折点
            continue;
        }
        else if(Data_Path_p->SideCoordinate[i][2] <Data_Path_p->SideCoordinate[i+6][2]&&Data_Path_p->SideCoordinate[i][2] <Data_Path_p->SideCoordinate[i-6][2]&&
        Data_Path_p->SideCoordinate[i][2] <Data_Path_p->SideCoordinate[i+5][2]&&Data_Path_p->SideCoordinate[i][2] <Data_Path_p->SideCoordinate[i-5][2]&&
        Data_Path_p->SideCoordinate[i][2]<=Data_Path_p->SideCoordinate[i+4][2]&&Data_Path_p->SideCoordinate[i][2] <=Data_Path_p->SideCoordinate[i-4][2]&&
        //Data_Path_p->SideCoordinate[i][2]<=Data_Path_p->SideCoordinate[i+3][2]&&Data_Path_p->SideCoordinate[i][2]<=Data_Path_p->SideCoordinate[i-3][2]&&
        abs(Data_Path_p->SideCoordinate[i][2]-Data_Path_p->SideCoordinate[i+2][2])<3&&
        abs(Data_Path_p->SideCoordinate[i][2]-Data_Path_p->SideCoordinate[i-2][2])<3&&
        abs(Data_Path_p->SideCoordinate[i+2][2]-Data_Path_p->SideCoordinate[i+4][2])<5&&
        abs(Data_Path_p->SideCoordinate[i-2][2]-Data_Path_p->SideCoordinate[i-4][2])<5&&
        abs(Data_Path_p->SideCoordinate[i+4][2]-Data_Path_p->SideCoordinate[i+5][2])<7&&
        abs(Data_Path_p->SideCoordinate[i-4][2]-Data_Path_p->SideCoordinate[i-5][2])<7&&
        Data_Path_p->SideCoordinate[i][2]!=237)
        {//就很暴力，这个数据是在前5，后5中最大的，那就是单调突变点
            startIndex=i;
            break;
        }
    }
    if(startIndex)
    {
        return startIndex;
    }
    else
        return 0;
}

inline int GetWhiteEnd(Data_Path *Data_Path_p) {
    int startIndex = -1;
    int Right_Count=0;
    for(int i=0;i<=Data_Path_p->NumSearch[1];i++)//会读取前5后5数据，所以前面对输入范围有要求
    {
        if(Data_Path_p->SideCoordinate_Eight[i][2]==317) Right_Count++;
        else break;
    }
    if(Right_Count>=30) Right_Count=30;
    return Right_Count;
}



#endif 
