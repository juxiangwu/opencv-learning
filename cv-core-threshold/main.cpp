#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    cv::Mat frame;
    char key = -1;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(7,7));
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return 0;
    }
    std::vector<cv::Mat> channels;
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }

        cv::split(frame,channels);

        cv::threshold(channels[0],channels[0],120,150,cv::THRESH_BINARY);
        cv::threshold(channels[1],channels[1],120,150,cv::THRESH_BINARY);
        cv::threshold(channels[2],channels[2],120,150,cv::THRESH_BINARY);

        cv::Mat dst;
        cv::merge(channels,dst);

        cv::imshow("src",frame);
        cv::imshow("dst-threshold",dst);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
