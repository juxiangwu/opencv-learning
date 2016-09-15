#include <iostream>
#include <opencv2/opencv.hpp>
#include "qrdecoderutils.h"
#include "bounded_buffer.h"
#include <boost/thread.hpp>

QRDecoderUtils g_scan_utils;
volatile bool g_is_running = true;
kkoolerter::bounded_buffer g_frame_buffer(4);

void scan_qr_func(){
    while(g_is_running){
        cv::Mat src = g_frame_buffer.receive();
        if(src.empty()){
            std::cerr << "received empty frame \n";
            continue;
        }
        std::vector<std::string> res = g_scan_utils.decodeFromMat(src);
        int size = res.size();
        for(int i = 0;i < size;i++){
            std::cout <<  res[i] << std::endl;
        }
    }
    std::cout << "scan finished.\n";
}

int main(int argc, char *argv[])
{
    //const std::string file("/home/jenson/Downloads/generate.jpg");
    //cv::Mat src = cv::imread(file);
   // QRDecoderUtils utils;
    //std::vector<std::string> res = utils.decodeFromMat(src);
    cv::VideoCapture cap(0);
    cv::Mat frame;
    if(!cap.isOpened()){
        std::cerr << "cannot open camera" << std::endl;
        g_is_running = true;
        return -1;
    }
    char key = -1;
    cv::namedWindow("src");
    cv::namedWindow("qr-image");
    cv::Rect rect(270,120,258,258);
    cv::Rect copy_rect(271,121,256,256);
    boost::thread scan_thread(&scan_qr_func);
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }

        cv::rectangle(frame,rect,cv::Scalar(0,0,255),1);
        cv::Mat qr_frame_image(frame,copy_rect);
        cv::cvtColor(qr_frame_image,qr_frame_image,cv::COLOR_RGB2GRAY);
        g_frame_buffer.send(qr_frame_image);
        cv::imshow("src",frame);
        cv::imshow("qr-image",qr_frame_image);
        key = cv::waitKey(10);
        if(key == 27){
            g_is_running = false;
            break;
        }
    }
    scan_thread.join();
    return 0;
}
