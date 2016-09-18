#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    cv::Mat src(128,128,CV_8UC3,cv::Scalar(10,20,128));
    IplImage * img = cvLoadImage("/home/jenson/Pictures/f1.jpg");
    cv::Mat dst = cv::cvarrToMat(img,false);
    cv::Mat m1 = (cv::Mat_<double>(3,3)<< 1,2,3,4,5,6,7,8,9);
    std::cout << m1 << std::endl;
    cv::imshow("dst",dst);
    cv::waitKey();
    cvReleaseImage(&img);
    return 0;
}
