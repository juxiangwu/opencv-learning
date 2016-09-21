#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static Scalar random_color(RNG& rng)
{
    int icolor = (unsigned)rng;

    return Scalar(icolor&0xFF, (icolor>>8)&0xFF, (icolor>>16)&0xFF);
}

void remove_background(cv::Mat & src,cv::Mat & background,cv::Mat & out){
    cv::Mat src32;
    src.convertTo(src32,CV_32F);
    cv::Mat bg32;
    background.convertTo(bg32,CV_32F);
    cv::Mat res = (1.0 - src32 / bg32) * 255;
    res.convertTo(out,CV_8U);
    cv::cvtColor(res,res,cv::COLOR_RGB2GRAY);
    cv::threshold(res,out,30,180,cv::THRESH_BINARY);
}

void process_segmentation(cv::Mat& src,cv::Mat & out){
    cv::Mat labels;
    int numobjects = cv::connectedComponents(src,labels);
    if(numobjects < 2){
        out = cv::Mat::zeros(src.size(),CV_8UC3);
        std::cout << "not found any objects\n";
        return;
    }
    out = cv::Mat::zeros(src.size(),CV_8UC3);
    cv::RNG rng(0xFFFFFFFF);
    std::cout << "found:" << numobjects << " objects" << std::endl;
    for(int i = 0;i < numobjects;i++){
        cv::Mat mask = labels == i;
        out.setTo(random_color(rng),mask);
    }

}

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
    cv::Mat frame;
    char key = -1;
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }

        cv::Mat removed_bg = frame.clone();
        remove_background(frame,background,removed_bg);
        cv::imshow("src",frame);
        cv::imshow("bg:thresh",removed_bg);
        removed_bg.convertTo(removed_bg,CV_8U);
        cv::Mat out = frame.clone();
        process_segmentation(removed_bg,out);


       cv::imshow("result",out);
        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
