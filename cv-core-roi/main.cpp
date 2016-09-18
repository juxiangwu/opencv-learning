#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera \n";
        return -1;
    }

    char key = -1;
    cv::Mat logo = cv::imread("/home/jenson/Pictures/matlab.png");
    cv::Rect roi(0,0,logo.cols,logo.rows);

    cv::Mat frame;

    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::Mat roi_frame = frame(roi);
        //cv::add(roi_frame,logo,roi_frame);
        cv::addWeighted(roi_frame,0.05,logo,0.995,2.5,roi_frame);
        cv::imshow("src",frame);
        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }

    return 0;
}
