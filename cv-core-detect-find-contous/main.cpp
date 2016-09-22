/**
 * @function findContours_Demo.cpp
 * @brief Demo code to find contours in an image
 * @author OpenCV team
 */

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
bool is_fill = false;

/// Function header
void thresh_callback(int, void* );

/**
 * @function main
 */
int main( int, char** argv )
{
    /// Load source image
    //  src = imread(argv[1]);
    //  if (src.empty())
    //  {
    //    cerr << "No image supplied ..." << endl;
    //    return -1;
    //  }

    cv::VideoCapture cap(1);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera\n";
        return -1;
    }
    const char* source_window = "Source";
    char key = -1;
    namedWindow( source_window, WINDOW_AUTOSIZE );
    namedWindow( "Contours", WINDOW_AUTOSIZE );

    /// Convert image to gray and blur it

    createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );


    /// Create Window
    while(true){
        cap >> src;
        if(src.empty()){
            continue;
        }
        imshow( source_window, src );
        cvtColor( src, src_gray, COLOR_BGR2GRAY );
        blur( src_gray, src_gray, Size(3,3) );
        thresh_callback( 0, 0 );

        key = waitKey(10);
        if(key == 27){
            break;
        }
        if(key == 'f'){
            is_fill = !is_fill;
        }
    }
    return(0);
}

/**
 * @function thresh_callback
 */
void thresh_callback(int, void* )
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    Canny( src_gray, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        if(!is_fill){
        drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
        }else{
            drawContours(drawing,contours,(int)i,color,CV_FILLED);
        }
    }

    /// Show in a window
    imshow( "Contours", drawing );
}
