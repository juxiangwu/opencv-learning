#include <iostream>
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;

void process_explosure(cv::Mat & inputImg,cv::Mat & outputImg,int strength){
    int row = inputImg.rows;
    int step = inputImg.step;
    uchar* pInputImg = inputImg.data;
    uchar* pOutputImg = outputImg.data;
    for (int i = 0; i < row * step; i++){
        pOutputImg[i] = (uchar)min<uchar>(255, (uchar)max<uchar>(0, pInputImg[i] * pow(2, strength)));
    }
}


int main(int argc, char *argv[])
{
    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera" << std::endl;
        return -1;
    }
    cv::Mat frame;
    char key = -1;
    std::vector<cv::Mat> channels;
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::split(frame,channels);
        cv::equalizeHist(channels[0],channels[0]);
        cv::equalizeHist(channels[1],channels[1]);
        cv::equalizeHist(channels[2],channels[2]);

        cv::Mat explosured;
        cv::merge(channels,explosured);

        cv::Mat dst = frame.clone();
        process_explosure(frame,dst,-2.0);

        cv::imshow("src",frame);
        cv::imshow("explosure",explosured);
        cv::imshow("dst",dst);
        key = cv::waitKey(10);
        if(key == 27){
            break;
        }

    }
    return 0;
}
