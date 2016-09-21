#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#define HAAR_CASCADE_FRONT_FACE_PATH "../../datas/haarcascade_frontalface_default.xml"//front face  haar特征方法
#define LBP_CASCADE_FRONT_FACE_PATH "../../datas/lbpcascade_frontalface.xml"//front face  LBP特征方法

using namespace std;
using namespace cv;

static cv::CascadeClassifier face_Classifier;
static cv::Mat src_gray;
static vector<Rect> face_rect;

bool face_detect(Mat& src,vector<Rect>& faceRect){
    faceRect.clear();
    cv::cvtColor( src, src_gray, CV_BGR2GRAY );//灰度化
    cv::equalizeHist( src_gray, src_gray );//直方图均衡，增加对比度以提高识别率
    //detect
    face_Classifier.detectMultiScale(src_gray, faceRect,
                                     1.1, 2, 0|CV_HAAR_SCALE_IMAGE,
                                     cv::Size(30, 30) );
    if(faceRect.size() == 0)
        return false;

    return true;
}

int main(int argc, char *argv[])
{
    bool res = face_Classifier.load(HAAR_CASCADE_FRONT_FACE_PATH);
    if(!res){
        std::cerr << "cannot load " << HAAR_CASCADE_FRONT_FACE_PATH << std::endl;
        return -1;
    }

    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return -1;
    }

    cv::Mat src;
    char key = -1;
    cv::namedWindow("src");
    while(true){
        cap >> src;
        if(src.empty()){
            continue;
        }
        bool detected = face_detect(src,face_rect);
        if(detected){
            for(int i = 0;i < face_rect.size();i++){
                cv::rectangle(src,face_rect[i],cv::Scalar(255,0,0));
            }
        }
        cv::imshow("src",src);
        key = cv::waitKey(10);

        if(key == 27){
            break;
        }
    }
    return 0;
}
