
/************************************************************************/
/*                           OpenCV使用不同方式访问图像数据                                   */
/************************************************************************/

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
#include <iostream>
using namespace std;

int main(int argc, char ** argv)
{
    //方法1：使用cvGet2D()函数访问
    double t = (double)getTickCount();
    Mat imgx(1000, 1000, CV_32F);
    IplImage imgIP = imgx;
    for (int i=0; i<1000; i++)
    {
        for (int j=0; j<1000; j++)
        {
            cvGet2D(&imgIP,i,j);
        }
    }
    t = (double)getTickCount() - t;
    printf("方法1 - Time: %gms\n", t*1000/getTickFrequency());

    //方法2：使用at操作符访问
    t = (double)getTickCount();
    Mat img1(1000, 1000, CV_32F);
    for (int i=0; i<1000; i++)
    {
        for (int j=0; j<1000; j++)
        {
            img1.at<float>(i,j) = 3.2f;
        }
    }
    t = (double)getTickCount() - t;
    printf("方法2 - Time: %gms\n", t*1000/getTickFrequency());

    //方法3：使用ptr指针访问
    t = (double)getTickCount();
    Mat img2(1000, 1000, CV_32F);
    for (int i=0; i<1000; i++)
    {
        for (int j=0; j<1000; j++)
        {
            img2.ptr<float>(i)[j] = 3.2f;
        }
    }
    t = (double)getTickCount() - t;
    printf("方法3  - Time: %gms\n", t*1000/getTickFrequency());

    //方法4：使用data数组访问
    t = (double)getTickCount();
    Mat img4(1000, 1000, CV_32F);
    for (int i=0; i<1000; i++)
    {
        for (int j=0; j<1000; j++)
        {
            ((float*)img4.data)[i*1000+j] = 3.2f;
        }
    }
    t = (double)getTickCount() - t;
    printf("方法4  - Time: %gms\n", t*1000/getTickFrequency());

    //方法5：使用指针访问
    t = (double)getTickCount();
    Mat img3(1000, 1000, CV_32F);
    float* pData = (float*)img3.data;
    for (int i=0; i<1000; i++)
    {
        for (int j=0; j<1000; j++)
        {
            *(pData) = 3.2f;
            pData++;
        }
    }
    t = (double)getTickCount() - t;
    printf("方法5  - Time: %gms\n", t*1000/getTickFrequency());
    //system("pause");
}

