#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera\n";
        return -1;
    }

    char key = -1;
    cv::Mat frame;

    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }

        cv::Mat box_filter_frame;
        cv::boxFilter(frame,box_filter_frame,CV_8UC3,cv::Size(7,7));
        cv::imshow("src",frame);
        cv::imshow("boxfilter",box_filter_frame);

        cv::Mat gaussian_frame;
        cv::GaussianBlur(frame,gaussian_frame,cv::Size(7,7),0,0);
        cv::imshow("gaussian-fitler",gaussian_frame);


        cv::Mat median_frame;
        cv::medianBlur(frame,median_frame,7);
        cv::imshow("median-filter",median_frame);

        cv::Mat bilateral_frame;
        cv::bilateralFilter(frame,bilateral_frame,25,25 * 2, 25 / 2);
        cv::imshow("bilateral-filter",bilateral_frame);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }

    return 0;
}
