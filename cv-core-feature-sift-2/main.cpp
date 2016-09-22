#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace std;
using namespace cv;

int main()
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera\n";
        return -1;
    }
    cv::Mat image;
    char key = -1;

    namedWindow("sift");

    vector<KeyPoint> keypoints;          //  存放关键点
    while(true){
        cap >> image;
        if(image.empty()){
            continue;
        }
        cv::Ptr<cv::xfeatures2d::SiftFeatureDetector> sift = cv::xfeatures2d::SiftFeatureDetector::create(0.03, 10.0);   // 其中0.03代表特征的阀值：用于去除低对比度的关键点   10是用于降低直线敏感度的阀值：去除不稳点的边缘响应点
        sift->detect(image, keypoints);
        drawKeypoints(image, keypoints, image, Scalar(255,255,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        imshow("sift", image);
        key = waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
