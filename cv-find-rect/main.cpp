#include <iostream>
#include "detectlabel.h"

using namespace std;

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera\n";
        return -1;
    }

    char key = -1;
    DetectLabel detectLabel;
    cv::Mat src;
    //std::vector < std::vector < cv::Point> > points;
    vector<cv::Mat> outputs;

    while(true){
        cap >> src;
        if(src.empty()){
            continue;
        }
        cv::Mat gray;
        cv::cvtColor(src,gray,cv::COLOR_RGB2GRAY);
        //points.clear();
        //detectLabel.findRect(gray,points);
        cv::imshow("src",src);
        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
        outputs.clear();
        detectLabel.segment(src,outputs);
        if(outputs.size() == 0){
            std::cerr << "not found any rects\n";
            continue;
        }

        int width = 0;
        int height = 0;
        for(int i = 0;i < outputs.size();i++){

            cv::Mat m = outputs[i];
            if(m.cols == 0 || m.rows == 0){
                continue;
            }
            width += m.cols + 10;
            int temp = max<int>(height,m.rows);
            height = temp;
        }
        if(width == 0 || height == 0){
            std::cerr <<"invalid image size\n";
            continue;
        }
        std::cout << "width = " << width << ",height = " << height << std::endl;
        cv::Mat output(cv::Size(width,height),outputs[0].type());
        output.setTo(cv::Scalar(0,0,0));
        int x = 0;
        int y = 0;
        for(int i = 0;i < outputs.size();i++){
            cv::Mat m = outputs[i];
            int tx = m.cols;
            int ty = m.rows;
            cv::Rect rect(x,y,tx,ty);
            cv::Mat region = output(rect);
            cv::add(region,outputs[i],region);
            x += tx + 10;
            //y += ty + 10;
        }

        cv::imshow("segments",output);

    }
    return 0;
}
