#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void drawDetectLines(Mat& image,const vector<Vec4i>& lines,Scalar & color)
{
    // 将检测到的直线在图上画出来
    vector<Vec4i>::const_iterator it=lines.begin();
    while(it!=lines.end())
    {
        Point pt1((*it)[0],(*it)[1]);
        Point pt2((*it)[2],(*it)[3]);
        line(image,pt1,pt2,color,2); //  线条宽度设置为2
        ++it;
    }
}

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera\n";
        return -1;
    }
    cv::Mat image;
    char key = -1;
    cv::Scalar color(0,0,255);
    namedWindow("Lines");
    while(true){
        cap >> image;
        if(image.empty()){
            continue;
        }
        Mat I;
        cvtColor(image,I,CV_BGR2GRAY);

        Mat contours;
        Canny(I,contours,125,350);
        threshold(contours,contours,128,255,THRESH_BINARY);
        vector<Vec4i> lines;
        // 检测直线，最小投票为90，线条不短于50，间隙不小于10
        HoughLinesP(contours,lines,1,CV_PI/180,50,30,10);
        drawDetectLines(image,lines,color);
        imshow("Lines",image);
        key = waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
