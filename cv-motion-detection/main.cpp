#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    VideoCapture capture("/home/jenson/Videos/sloat.mp4");//capture(0);
    if(!capture.isOpened())
        return -1;
    double rate = capture.get(CV_CAP_PROP_FPS);
    int delay = 1000/rate;
    Mat framePro,frame,dframe;
    bool flag = false;
    namedWindow("image",CV_WINDOW_AUTOSIZE);
    namedWindow("test",CV_WINDOW_AUTOSIZE);
    char key = -1;
   // while(true){
    while(capture.read(frame)){
        //capture >> frame;
        if(frame.empty()){
            continue;
        }
        if(false == flag)
        {
            framePro = frame.clone();
            flag = true;
        }
        else
        {
            absdiff(frame,framePro,dframe);
            framePro = frame.clone();
            threshold(dframe,dframe,80,255,CV_THRESH_BINARY);
            imshow("image",frame);
            imshow("test",dframe);

        }
        key = waitKey(delay);
        if(key == 27){
            break;
        }
    }
    return 0;
}
