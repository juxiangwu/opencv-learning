#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    cv::Mat frame;
    char key = -1;
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return 0;
    }
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
       // cv::Mat dist;
        cv::Rect rect;
        cv::floodFill(frame,cv::Point(50,300),cv::Scalar(155,255,15),
                      &rect,cv::Scalar(20,20,20),cv::Scalar(20,20,20));
        cv::imshow("src",frame);
       // cv::imshow("floodfill",dist);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
