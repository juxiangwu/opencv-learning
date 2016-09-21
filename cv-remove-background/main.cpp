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

    cv::Mat background = cv::imread("../../datas/simple-bg.jpg");
    if(background.empty()){
        std::cerr << "cannot read background image.\n";
        return -1;
    }

    cv::Mat bg32;
    background.convertTo(bg32,CV_32F);

    cv::Mat frame;
    char key = -1;
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::Mat frame32;
        frame.convertTo(frame32,CV_32F);
        cv::Mat result = (frame32 / bg32) * 255;
        result.convertTo(result,CV_8U);
        //cv::Mat result =  (background - frame);
        cv::imshow("src",frame);
        cv::imshow("result",result);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }

    return 0;
}
