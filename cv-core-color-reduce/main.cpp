#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

static const int TYPE_1 = 0x01;
static const int TYPE_2 = 0x02;
static const int TYPE_3 = 0x03;

void color_reduce_type_1(cv::Mat& input,cv::Mat & output,int div){
    int cols = input.cols * input.channels();
    int rows = input.rows;

    for(int i = 0;i < rows;i++){
        uchar * src_data = input.ptr<uchar>(i);
        uchar * out_data = output.ptr<uchar>(i);
        for(int j = 0;j < cols;j++){
            out_data[j] = src_data[j] / div * div + div / 2;
        }
    }
}

void color_reduce_type_2(cv::Mat & input,cv::Mat & output,int div){

}

void color_reduce_type_3(cv::Mat & input,cv::Mat & output,int div){

}

void color_reduce(cv::Mat & input,cv::Mat & output,int div,int type){
    switch(type){
    case TYPE_1:
        color_reduce_type_1(input,output,div);
        break;
    case TYPE_2:
        color_reduce_type_2(input,output,div);
        break;
    case TYPE_3:
        color_reduce_type_3(input,output,div);
        break;
    }
}

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera.\n";
        return 0;
    }

    cv::Mat frame;
    char key = -1;
    bool is_running = true;
    int div = 32;
    int type = TYPE_1;
    while(is_running){
        cap >> frame;
        if(frame.empty()){
            continue;
        }

        cv::Mat dst = frame.clone();
        cv::imshow("src",frame);


        key = cv::waitKey(10);
        switch(key){
        case 'a':
            type = TYPE_1;
            std::cout << "select type 1 \n";
            break;
        case 'b':
            type = TYPE_2;
            std::cout << "select type 1 \n";
            break;
        case 'c':
            type = TYPE_3;
            break;
        case 'q':
            div++;
            if(div >= 256){
                div = 256;
            }
            std::cout << "div = " << div << std::endl;
            break;
        case 'w':
            div--;
            if(div <= 0){
                div = 1;
            }
            std::cout << "div = " << div << std::endl;

            break;
        case 27:
            is_running = false;
            break;
        }
        color_reduce(frame,dst,div,type);
        cv::imshow("color-reduce",dst);
    }
    return 0;
}
