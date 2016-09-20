#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

int main(int argc, char *argv[])
{

    cv::Mat r1 = cv::imread("../../datas/r1.jpg");
    cv::Mat r2 = cv::imread("../../datas/r1.jpg");
    cv::Mat r = r1 < 180;
    cv::imshow("mask",r);
    cv::waitKey();
    return 0;
}
