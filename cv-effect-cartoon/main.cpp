#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void process_cartoon(cv::Mat & img,cv::Mat & result){
    Mat imgMedian;
    medianBlur(img, imgMedian, 7);

    // Detect edges with canny
    Mat imgCanny;
    Canny(imgMedian, imgCanny, 50, 150);

    // Dilate the edges
    Mat kernel= getStructuringElement(MORPH_RECT, Size(2,2));
    dilate(imgCanny, imgCanny, kernel);

    // Scale edges values to 1 and invert values
    imgCanny= imgCanny/255;
    imgCanny= 1-imgCanny;
    // Use float values to allow multiply between 0 and 1
    Mat imgCannyf;
    imgCanny.convertTo(imgCannyf, CV_32FC3);
    // Blur the edgest to do smooth effect
    blur(imgCannyf, imgCannyf, Size(5,5));

    // Apply bilateral filter to homogenizes color
    Mat imgBF;
    bilateralFilter(img, imgBF, 9, 150.0, 150.0);
    // truncate colors
    result= imgBF/25;
    result= result*25;
    /** MERgES COLOR + EDgES **/
    // Create a 3 channles for edges
    Mat imgCanny3c;
    Mat cannyChannels[]={ imgCannyf, imgCannyf, imgCannyf};
    merge(cannyChannels, 3, imgCanny3c);
    // Convert color result to float
    Mat resultf;
    result.convertTo(resultf, CV_32FC3);
    // Multiply color and edges matrices
    multiply(resultf, imgCanny3c, resultf);
    // convert to 8 bits color
    resultf.convertTo(result, CV_8UC3);
}

int main(int argc, char *argv[])
{
    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera" << std::endl;
        return -1;
    }
    cv::Mat frame;
    char key = -1;
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }
        cv::Mat result = frame.clone();
        process_cartoon(frame,result);
        cv::imshow("src",frame);
        cv::imshow("src-halo",result);
        key = cv::waitKey(10);
        if(key == 27){
            break;
        }
    }
    return 0;
}
