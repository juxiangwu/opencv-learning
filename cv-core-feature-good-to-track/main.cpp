
/**
* @使用Shi-Tomasi方法进行角点检测
* @author holybin
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;

/// 全局变量
Mat src, srcGray;
int maxCorners = 23;	//角点个数的最大值
int maxCornersThresh = 1000;	//角点个数最大值的上限（滑动条范围0-1000）

RNG rng(12345);
char* sourceWindow = "src";

/// 角点检测函数声明
void doGoodFeaturesToTrack( int, void* );

int main( int argc, char** argv )
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return -1;
    }

    //cv::Mat src;
    char key = -1;

    /// 创建显示窗口以及滑动条
    namedWindow( sourceWindow, CV_WINDOW_AUTOSIZE );
    createTrackbar( "max num:", sourceWindow, &maxCorners, maxCornersThresh, doGoodFeaturesToTrack );
    while(true){
        cap >> src;
        if(src.empty()){
            continue;
        }
        cvtColor( src, srcGray, CV_BGR2GRAY );
        imshow( sourceWindow, src );

        doGoodFeaturesToTrack( 0, 0 );

        key = waitKey(10);
        if(key == 27){
            break;
        }
    }
    return(0);
}

/// 角点检测函数实现：标示出每个角点位置
void doGoodFeaturesToTrack( int, void* )
{
    if( maxCorners < 1 )
        maxCorners = 1;

    /// Shi-Tomasi的参数设置
    vector<Point2f> corners;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3;
    bool useHarrisDetector = false;	//不使用Harris检测算法
    double k = 0.04;

    /// 深度拷贝原图像用于绘制角点
    Mat srcCopy = src.clone();
    /// 应用角点检测算法
    goodFeaturesToTrack( srcGray,
                         corners,
                         maxCorners,
                         qualityLevel,
                         minDistance,
                         Mat(),	//未选择感兴趣区域
                         blockSize,
                         useHarrisDetector,
                         k );

    /// 当maxCorners的值较小时，以下两个值基本是一样的；
    /// 当maxCorners的值较大时，实际检测到的角点数目有可能小于maxCorners，以下两个值不一样。
    cout<<"*  detected corners : "<<corners.size()<<endl;
    cout<<"** max corners: "<<maxCorners<<endl;

    /// 绘制出角点
    int r = 4;
    for( int i = 0; i < corners.size(); i++ )
        circle( srcCopy, corners[i], r, Scalar(0,255,0), -1, 8, 0 );

    /// 显示结果
    namedWindow( sourceWindow, CV_WINDOW_AUTOSIZE );
    imshow( sourceWindow, srcCopy );
}

