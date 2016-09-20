#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    cv::Mat depthImage;
    depthImage = cv::imread("/home/jenson/Pictures/f1.png", CV_LOAD_IMAGE_ANYDEPTH  | CV_LOAD_IMAGE_GRAYSCALE ); // Read the file
    depthImage.convertTo(depthImage, CV_32F); // convert the image data to float type
    namedWindow("window");
    float max = 0;
    for(int i = 0; i < depthImage.rows; i++){
        for(int j = 0; j < depthImage.cols; j++){
            if(depthImage.at<float>(i,j) > max){
                max = depthImage.at<float>(i,j);
            }
        }
    }
    cout << max << endl;


    float divisor = max / 255.0;
    cout << divisor << endl;
    for(int i = 0; i < depthImage.rows; i++){
        for(int j = 0; j < depthImage.cols; j++){
            cout << depthImage.at<float>(i,j) << ", ";
            max = depthImage.at<float>(i,j) /= divisor;
            cout << depthImage.at<float>(i,j) << endl;
        }
    }


    imshow("window", depthImage);
    waitKey(0);
    return 0;
}
