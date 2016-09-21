#include <opencv2/opencv.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc,char * argv[])
{
    // Mat image = imread("church01.jpg");
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return -1;
    }
    cv::Mat frame;
    char key = -1;


   // initModule_nonfree();
    Ptr<cv::xfeatures2d::SIFT> sift1 = cv::xfeatures2d::SIFT::create();
    //sift1->set("contrastThreshold", 0.01f);

    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }

        Mat imageGray;
        Mat descriptors;
        vector<KeyPoint> keypoints;

        cv::cvtColor(frame,imageGray,cv::COLOR_RGB2GRAY);

        //(*sift1)(imageGray, noArray(), keypoints, descriptors);
        sift1->detect(imageGray, keypoints, descriptors);
        cv::drawKeypoints(frame, keypoints, frame, Scalar(255,0,0));
        cv::imshow("src",frame);
        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }

    return 0;
}

