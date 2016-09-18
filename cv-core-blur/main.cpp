#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

static int g_kernel_size = 7;
static cv::Size g_blur_kernel_size(g_kernel_size,g_kernel_size);
void onTrackbarCallback(int value, void* userdata){
    if(value <= 0){
        value = 1;
    }
    g_blur_kernel_size = cv::Size(value,value);
}

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera \n";
        return 0;
    }
    cv::namedWindow("src");
    cv::namedWindow("blur");
    cv::createTrackbar("blur-value","blur",&g_kernel_size,15,&onTrackbarCallback);

    char key = -1;
    cv::Mat frame;
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::Mat dst;
        cv::blur(frame,dst,g_blur_kernel_size);
        cv::imshow("src",frame);
        cv::imshow("blur",dst);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
