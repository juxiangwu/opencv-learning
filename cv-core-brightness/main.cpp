#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

static int g_brightness = 10;
static float g_brightness_alpha = 5;
int default_brightness_alpha = 5;

void on_brightness_trackbar_changed(int val,void * usrdata){
    g_brightness = val;
}


void on_brightness_alpha_trackbar_changed(int val,void * usrdata){
    if(g_brightness_alpha == 0){
        g_brightness_alpha = 0.01f;
    }
    g_brightness_alpha = val / 10.0f;
}

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return -1;
    }
    cv::Mat frame;
    cv::namedWindow("src");
    cv::namedWindow("src-brightness");
    cv::createTrackbar("Brightness","src-brightness",&g_brightness,128,on_brightness_trackbar_changed);
    cv::createTrackbar("Alpha","src-brightness",&default_brightness_alpha,10,on_brightness_alpha_trackbar_changed);
    char key = -1;
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::Mat dst = g_brightness_alpha * frame + g_brightness;
        cv::imshow("src",frame);
        cv::imshow("src-brightness",dst);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }

    return 0;
}
