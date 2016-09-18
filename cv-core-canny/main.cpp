#include <iostream>
#include <opencv2/opencv.hpp>

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

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
