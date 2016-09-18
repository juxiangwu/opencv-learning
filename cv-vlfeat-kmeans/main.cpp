#include <iostream>
#include <opencv2/opencv.hpp>

extern "C"{
#include <vl/ikmeans.h>
}
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{

    int row = 255;
    int col = 255;
    Mat show = Mat::zeros(row, col, CV_8UC3);
    Mat show2 = show.clone();


    int data_num = 200;
    int data_dim = 2;
    vl_uint8 *data = new vl_uint8[data_num * data_dim];

    for( int i=0; i<data_num; ++i)
    {
        vl_uint8 x = data[i*data_dim] = rand()% col;
        vl_uint8 y = data[i*data_dim+1] = rand()% row;
        circle(show,Point(x,y),2,Scalar(255,255,255));
    }
    std::cout << "finished init data\n";
    cv::imshow("source-data",show);

    VlIKMFilt *kmeans = vl_ikm_new(VL_IKM_ELKAN);
    vl_uint K = 3;
    vl_ikm_init_rand(kmeans, data_dim, K);
    vl_ikm_train(kmeans, data, data_num);
    vl_uint * label = new vl_uint[data_num];
    vl_ikm_push(kmeans, label, data, data_num);
    std::cout << "finished kmeans trainning \n";

    for( int i=0;i<data_num; ++i)
    {
        vl_uint8 x = data[i*data_dim];
        vl_uint8 y = data[i*data_dim+1];
        switch(label[i])
        {
        case 0:
            circle(show2,Point(x,y),2,Scalar(255,0,0));
            break;
        case 1:
            circle(show2,Point(x,y),2,Scalar(0,255,0));
            break;
        case 2:
            circle(show2,Point(x,y),2,Scalar(0,0,255));
            break;
        }
    }

    cv::imshow("kmeans-result",show2);

    cv::waitKey();

    return 0;
}
