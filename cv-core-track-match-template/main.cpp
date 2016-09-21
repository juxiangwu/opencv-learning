
/**
 * Object tracking using function matchTemplate
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;

/// 全局变量 ///
Rect templRect;						//模板矩形框
bool drawing_rect = false;		//是否绘制矩形框
const char* windowName = "Object Tracker";	//窗口名称

/// 绘制模板矩形框 ///
void mouseHandler(int event, int x, int y, int flags, void *param)
{
    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        drawing_rect = true;
        //初始位置
        templRect = Rect( x, y, 0, 0 );
        break;
    case CV_EVENT_LBUTTONUP:
        drawing_rect = false;
        //从右往左
        if( templRect.width < 0 )
        {
            templRect.x += templRect.width;
            templRect.width *= -1;
        }
        //从下往上
        if( templRect.height < 0 )
        {
            templRect.y += templRect.height;
            templRect.height *= -1;
        }
        break;
    case CV_EVENT_MOUSEMOVE:
        if (drawing_rect)
        {
            //从左往右，从上往下
            templRect.width = x - templRect.x;
            templRect.height = y - templRect.y;
        }
        break;
    }
}

/// 函数声明 ///
void tracking(Mat frame, Mat &templ, Rect &rect);

int main(int argc, char * argv[])
{
    //读取视频
    VideoCapture capture(1);
    if (!capture.isOpened())
    {
        cout << "capture device failed to open!" << endl;
        return -1;
    }

    //注册鼠标事件回调函数
    namedWindow(windowName, CV_WINDOW_AUTOSIZE);
    setMouseCallback(windowName, mouseHandler, NULL );

    //显示第一帧
    Mat frame, grayFrame;
    capture >> frame;
    imshow(windowName, frame);
    //等待绘制模板矩形框
    waitKey(0);

    //提取模板矩形框区域作为模板图像
    cvtColor(frame, grayFrame, CV_RGB2GRAY);	//灰度化
    Mat templ = grayFrame(templRect);

    //持续跟踪
    int countFrame = 0;
    while (1)
    {
        capture >> frame;
        if (frame.empty())	break;
        countFrame++;
        //计时
        double start = (double)getTickCount();

        //基于模板匹配的跟踪过程
        //参数：帧图像，模板图像，模板矩形框
        tracking(frame, templ, templRect);

        //显示
        rectangle(frame, templRect, Scalar(0, 0, 255), 3);
        imshow(windowName, frame);

        //计时
        double end = (double)getTickCount();
        cout << "Frame: "<<countFrame<<"\tTime: " << (end - start)*1000 / (getTickFrequency()) <<"ms"<< endl;
        if (waitKey(1) == 27)	break;
    }
    return 0;
}

/// 函数定义 ///
//根据上一个模板矩形框rect来设定当前搜索窗口searchWindow，
//并通过匹配（matchTemplate）搜索窗口searchWindow与当前模板图像templ来找到最佳匹配，
//最后更新模板矩形框rect与模板图像templ。
void tracking(Mat frame, Mat &templ, Rect &rect)
{
    Mat grayFrame;
    cvtColor(frame, grayFrame, CV_RGB2GRAY);

    //选取搜索窗口
    Rect searchWindow;
    searchWindow.width = rect.width * 3;
    searchWindow.height = rect.height * 3;
    searchWindow.x = rect.x + rect.width * 0.5 - searchWindow.width * 0.5;
    searchWindow.y = rect.y + rect.height * 0.5 - searchWindow.height * 0.5;
    searchWindow &= Rect(0, 0, frame.cols, frame.rows);

    //模板匹配
    Mat similarity;
    matchTemplate(grayFrame(searchWindow), templ, similarity, CV_TM_CCOEFF_NORMED);
    //找出最佳匹配的位置
    double maxVal;
    Point maxLoc;
    minMaxLoc(similarity, 0, &maxVal, 0, &maxLoc);

    //更新模板矩形框
    rect.x = maxLoc.x + searchWindow.x;
    rect.y = maxLoc.y + searchWindow.y;
    //更新模板
    templ = grayFrame(rect);
}
