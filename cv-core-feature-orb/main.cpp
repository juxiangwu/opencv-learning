/**
* @概述：采用ORB算子进行特征检测与特征值提取，并使用BruteForce匹配法进行特征点的匹配
* @类和函数：ORB + BruteForceMatcher
* @author：holybin
*/

#include "opencv2/core/core.hpp"
//#include "opencv2/nonfree/features2d.hpp"   //SurfFeatureDetector实际在该头文件中
//#include "opencv2/legacy/legacy.hpp"    //BruteForceMatcher实际在该头文件中
//#include "opencv2/features2d/features2d.hpp"  //FlannBasedMatcher实际在该头文件中
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

int main()
{
    Mat img_1 = imread("starbucks1.jpg");
    Mat img_2 = imread("starbucks2.jpg");
    if (!img_1.data || !img_2.data)
    {
        cout << "error reading images " << endl;
        return -1;
    }

    //1.orb检测特征点并提取特征值
    Ptr<ORB> orb = ORB::create();
    vector<KeyPoint> keyPoints_1, keyPoints_2;
    Mat descriptors_1, descriptors_2;
    orb->detect(img_1, Mat(), keyPoints_1, descriptors_1,);

    orb->detect(img_2, Mat(), keyPoints_2, descriptors_2);

    //2.BruteForceMatcher匹配
    FlannBasedMatcher matcher;  //也可以使用ruteForce<Hamming>
    vector<DMatch> matches;
    matcher.match(descriptors_1, descriptors_2, matches);

    //3.过滤匹配点
    double max_dist = 0; double min_dist = 100;
    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }
    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );
    //-- Draw only "good" matches (i.e. whose distance is less than 0.6*max_dist )
    //-- PS.- radiusMatch can also be used here.
    std::vector< DMatch > good_matches;
    for( int i = 0; i < descriptors_1.rows; i++ )
    {
        if( matches[i].distance < 0.6*max_dist )
        {
            good_matches.push_back( matches[i]);
        }
    }

    //4-1.绘制匹配点
    Mat img_matches;
    drawMatches(img_1, keyPoints_1, img_2, keyPoints_2,
        good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //4-2.根据匹配点绘制目标位置
    // localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    for (size_t i = 0; i < good_matches.size(); ++i)
    {
        // get the keypoints from the good matches
        obj.push_back(keyPoints_1[ good_matches[i].queryIdx ].pt);
        scene.push_back(keyPoints_2[ good_matches[i].trainIdx ].pt);
    }
    Mat H = findHomography( obj, scene, CV_RANSAC );
    // get the corners from the image_1
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( img_1.cols, 0);
    obj_corners[2] = cvPoint( img_1.cols, img_1.rows);
    obj_corners[3] = cvPoint( 0, img_1.rows);
    std::vector<Point2f> scene_corners(4);

    perspectiveTransform( obj_corners, scene_corners, H);

    // draw lines between the corners (the mapped object in the scene - image_2)
    line( img_matches, scene_corners[0] + Point2f( img_1.cols, 0), scene_corners[1] + Point2f( img_1.cols, 0),Scalar(255,0,0));
    line( img_matches, scene_corners[1] + Point2f( img_1.cols, 0), scene_corners[2] + Point2f( img_1.cols, 0),Scalar(255,0,0));
    line( img_matches, scene_corners[2] + Point2f( img_1.cols, 0), scene_corners[3] + Point2f( img_1.cols, 0),Scalar(255,0,0));
    line( img_matches, scene_corners[3] + Point2f( img_1.cols, 0), scene_corners[0] + Point2f( img_1.cols, 0),Scalar(255,0,0));
    imshow( "Match", img_matches);

    cvWaitKey();
    return 0;
}
