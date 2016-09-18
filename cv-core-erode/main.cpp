#include <iostream>
#include <opencv2/opencv.hpp>

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
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::Mat dst;
        cv::erode(frame,dst,kernel);
        cv::imshow("src",frame);
        cv::imshow("dist:erode",dst);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
