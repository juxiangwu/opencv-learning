#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return 0;
    }
    char key = -1;
    cv::Mat frame;
    std::vector<cv::Mat> channels;
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::Mat gray;
        cv::cvtColor(frame,gray,cv::COLOR_RGB2GRAY);
        cv::blur(gray,gray,cv::Size(3,3));
        cv::Canny(gray,gray,10,80);
        cv::imshow("src",frame);
        cv::imshow("Canny",gray);

        cv::split(frame,channels);
        cv::blur(channels[0],channels[0],cv::Size(7,7));
        cv::blur(channels[1],channels[1],cv::Size(7,7));
        cv::blur(channels[2],channels[2],cv::Size(7,7));
        cv::Canny(channels[0],channels[0],10,80);
        cv::Canny(channels[1],channels[1],10,80);
        cv::Canny(channels[2],channels[2],10,80);

        cv::Mat dst;
        cv::merge(channels,dst);
        cv::imshow("color-canny",dst);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
