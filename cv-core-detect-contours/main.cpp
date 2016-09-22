#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int apos = 80;
void on_track_callback(int val,void * data){
    apos = val;
}
int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return -1;
    }

    cv::Mat frame;
    char key = -1;
    std::vector<vector<cv::Point> > points;
    cv::Mat img_contours;
    cv::namedWindow("src");
    cv::createTrackbar("thresh","src",&apos,255);
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        if(img_contours.empty()){
            img_contours = cv::Mat(frame.size(),CV_8U);
        }
        cv::Mat img_gray;
        cv::Mat img_thresh;
        cv::Mat img_erizh;
        points.clear();
        cv::cvtColor(frame,img_gray,cv::COLOR_RGB2GRAY);
        cv::equalizeHist(img_gray,img_erizh);
        cv::threshold(img_erizh,img_thresh,apos,255,cv::THRESH_BINARY);
        cv::findContours(img_thresh,points,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        img_contours.setTo(cv::Scalar(0,0,0));
        cv::drawContours(img_contours,points,-1,cv::Scalar(255,0,0));
        cv::imshow("src",frame);
        cv::imshow("contours",img_contours);

        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }


    return 0;
}
