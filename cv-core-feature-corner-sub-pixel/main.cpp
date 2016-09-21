/**
* @使用cornerSubPix()函数在goodFeaturesToTrack()的角点检测基础上将角点位置精确到亚像素级别
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
int maxCorners = 10;	//角点个数的最大值
int maxCornersThresh = 20;	//角点个数最大值的上限（滑动条范围0-20）
char* detectWindow = "detection";
char* refineWindow = "refinement";

/// 角点精准化函数声明
void doCornerRefinement( int, void* );

int main( int argc, char** argv )
{
    /// 载入图像并灰度化
    //src = imread( "D:\\opencv_pic\\house_small.jpg", 1 );

    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera \n";
        return -1;
    }

    /// 创建显示窗口以及滑动条
    namedWindow( detectWindow, CV_WINDOW_AUTOSIZE );
    createTrackbar( "max  num:", detectWindow, &maxCorners, maxCornersThresh, doCornerRefinement );
    char key = -1;
    while(true){
        cap >> src;
        if(src.empty()){
            continue;
        }
        cvtColor( src, srcGray, CV_BGR2GRAY );
        imshow( detectWindow, src );

        doCornerRefinement( 0, 0 );

        key = waitKey(10);
        if(key == 27){
            break;
        }
    }
    return(0);
}

/// 使用Shi-Tomasi方法检测角点，再对角点位置进行精准化
void doCornerRefinement( int, void* )
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
    Mat detectSrcCopy = src.clone();
    Mat refineSrcCopy = src.clone();

    /// 应用Shi-Tomasi角点检测算法
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

    /// 1--显示角点检测结果
    int r = 3;
    cout<<"-- Before refinement: "<<endl;
    for( int i = 0; i < corners.size(); i++ )
    {
        // 标示出角点
        circle( detectSrcCopy, corners[i], r, Scalar(255,0,255), -1, 8, 0 );
        // 输出角点坐标
        cout<<"	["<<i<<"]  ("<<corners[i].x<<","<<corners[i].y<<")"<<endl;
    }
    namedWindow( detectWindow, CV_WINDOW_AUTOSIZE );
    imshow( detectWindow, detectSrcCopy );

    /// 角点位置精准化参数
    Size winSize = Size( 5, 5 );
    Size zeroZone = Size( -1, -1 );
    TermCriteria criteria = TermCriteria(
                CV_TERMCRIT_EPS + CV_TERMCRIT_ITER,
                40, //maxCount=40
                0.001 );	//epsilon=0.001
    /// 计算精准化后的角点位置
    cornerSubPix( srcGray, corners, winSize, zeroZone, criteria );

    /// 2--显示精准化后的角点
    cout<<"-- After refinement: "<<endl;
    for( int i = 0; i < corners.size(); i++ )
    {
        // 标示出角点
        circle( refineSrcCopy, corners[i], r, Scalar(255,0,255), -1, 8, 0 );
        // 输出角点坐标
        cout<<"	["<<i<<"]  ("<<corners[i].x<<","<<corners[i].y<<")"<<endl;
    }
    namedWindow( refineWindow, CV_WINDOW_AUTOSIZE );
    imshow( refineWindow, refineSrcCopy );

    cout<<endl;
}
