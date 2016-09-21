#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;

const double exponential_e = std::exp(1.0);
cv::Mat lut(1, 256, CV_8UC1);

void init_params(){
    for (int i=0; i<256; i++)
    {
        float x= (float)i/256.0;
        lut.at<uchar>(i)= cvRound( 256 * (1/(1 + pow(exponential_e,
                                                     -((x-0.5)/0.1)) )) );
    }
}

void process_halo(cv::Mat & img,cv::Mat & result){
    vector<cv::Mat> bgr;
    cv::split(img, bgr);
    cv::LUT(bgr[2], lut, bgr[2]);
    // merge result
    cv::merge(bgr, result);
    // Create image for halo dark
    cv::Mat halo( img.rows, img.cols, CV_32FC3, cv::Scalar(0.0,0.0,1.0) );
    // Create circle
    cv::circle(halo, cv::Point(img.cols/2, img.rows/2), img.cols/3,
               cv::Scalar(1,1,1), -1);
    cv::blur(halo, halo, cv::Size(img.cols/3, img.cols/3));
    // Convert the result to float to allow multiply by 1 factor
    cv::Mat resultf;
    result.convertTo(resultf, CV_32FC3);
    // Multiply our result with halo
    cv::multiply(resultf, halo, resultf);
    // convert to 8 bits
    resultf.convertTo(result, CV_8UC3);
}

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera" << std::endl;
        return -1;
    }
    init_params();
    cv::Mat frame;
    char key = -1;
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::Mat result = frame.clone();
        process_halo(frame,result);
        cv::imshow("src",frame);
        cv::imshow("src-halo",result);
        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
