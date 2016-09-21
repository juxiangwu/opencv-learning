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
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }

        cv::imshow("src",frame);
        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
        if(key == 'c'){
            cv::imwrite("out.jpg",frame);
        }
    }
    return 0;
}
