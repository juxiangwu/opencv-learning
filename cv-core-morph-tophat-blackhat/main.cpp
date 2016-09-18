#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return -1;
    }

    cv::Mat frame;
    char key = -1;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(10,10));

    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::Mat dst;
        cv::morphologyEx(frame,dst,cv::MORPH_TOPHAT,element);
        cv::imshow("src",frame);
        cv::imshow("dst-tophat",dst);

        cv::Mat dst2;
        cv::morphologyEx(frame,dst2,cv::MORPH_BLACKHAT,element);
        cv::imshow("src",frame);
        cv::imshow("dst-blackhat",dst2);

        cv::Mat dst3;
        cv::morphologyEx(frame,dst3,cv::MORPH_GRADIENT,element);
        cv::imshow("src",frame);
        cv::imshow("dst-gradient",dst3);


        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }

    return 0;
}
