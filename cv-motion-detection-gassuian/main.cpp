#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    VideoCapture cam(0);// 0打开默认的摄像头
    if(!cam.isOpened())
        return -1;
    namedWindow("mask",CV_WINDOW_AUTOSIZE);
    namedWindow("frame",CV_WINDOW_AUTOSIZE);
    Mat frame,mask,threImage,output;
    int delay = 1000/cam.get(CV_CAP_PROP_FPS);
    Ptr<BackgroundSubtractorMOG2> bgSubtractor  = createBackgroundSubtractorMOG2();
    //构造混合高斯模型 参数1：使用历史帧的数量 2：混合高斯个数，3：背景比例 4：:噪声权重
    char key = -1;
    while (true)
    {
        cam>>frame;
        imshow("frame",frame);
        bgSubtractor->apply(frame,mask);
        imshow("mask",mask);
        key = waitKey(delay);
        if(key == 27){
            break;
        }
    }
    return 0;
}
