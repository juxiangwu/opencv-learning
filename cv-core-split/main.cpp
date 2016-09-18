#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera\n";
        return -1;
    }
    cv::Mat frame;
    char key = -1;
    vector<cv::Mat> channels;
    while(true){

        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::split(frame,channels);
        cv::imshow("src-R",channels[0]);
        cv::imshow("src-G",channels[1]);
        cv::imshow("src-B",channels[2]);

        cv::Mat src;
        cv::merge(channels,src);
        cv::imshow("src",src);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
