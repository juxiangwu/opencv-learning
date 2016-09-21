/**
* @概述: 使用FAST进行特征点检测
* @类和函数: FAST函数，FastFeatureDetector类
* @author: holybin
*/
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return -1;
    }
    cv::Mat img;
    char key = -1;


    vector<KeyPoint> keypoints;
    while(true){
        cap >> img;
        if(img.empty()){
            continue;
        }
        FAST(img, keypoints, 20);

        //-- 绘制特征点
        Mat img_keypoints;
        drawKeypoints(img, keypoints, img_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
        //-- 显示特征点
        imshow("Keypoints", img_keypoints);
        //imwrite("fast_detection_result.png", img_keypoints);

        key = waitKey(10);
        if(key == 27){
            break;
        }
        keypoints.clear();
    }
    return 0;
}
