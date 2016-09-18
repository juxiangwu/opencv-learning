#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cstdio>
#include <iostream>
#include <fstream>
#include <pcl/common/common_headers.h>
#include <pcl/io/io.h>
#include <pcl/visualization//pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/visualization/cloud_viewer.h>
#include <vector>
#include <boost/filesystem.hpp>

using namespace cv;
using namespace std;
using namespace pcl;


ofstream out("points.txt");

boost::shared_ptr<pcl::visualization::PCLVisualizer> createVisualizer (pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
    viewer->setBackgroundColor (0, 0, 0);
    pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
    viewer->addPointCloud<pcl::PointXYZRGB> (cloud, rgb, "reconstruction");
    //viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "reconstruction");
    viewer->addCoordinateSystem ( 1.0 );
    viewer->initCameraParameters ();
    return (viewer);
}

int main()
{
    Mat img1, img2;
    const char * cwd = boost::filesystem::current_path().c_str();
    std::cout << cwd << std::endl;
    img1 = imread("datas/I1.png");
    std::cout <<"img1" << img1.size() << std::endl;
    img2 = imread("datas/I2.png");
    std::cout<<"img2" << img2.size() << std::endl;

    Mat g1,g2, disp, disp8;

    cvtColor(img1, g1, CV_BGR2GRAY);
    cvtColor(img2, g2, CV_BGR2GRAY);

    int sadSize = 3;

    cv::Ptr<StereoSGBM> sbm = StereoSGBM::create(0,16,3);
    sbm->setNumDisparities(144);
    sbm->setPreFilterCap(10);
    sbm->setMinDisparity(0);
    sbm->setUniquenessRatio(10);
    sbm->setSpeckleWindowSize(100);
    sbm->setSpeckleRange(32);
    sbm->setDisp12MaxDiff(1);
    sbm->setP1(sadSize*sadSize*4);
    sbm->setP2(sadSize*sadSize*32);
    sbm->compute(g1,g2,disp);
    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);

    Mat dispSGBMscale;
    disp.convertTo(dispSGBMscale,CV_32F, 1./16);

    double cm1[3][3] = {{8.941981e+02, 0.000000e+00, 6.601406e+02}, {0.000000e+00, 8.927151e+02, 2.611004e+02}, {0.000000e+00, 0.000000e+00, 1.000000e+00}};
    double cm2[3][3] = {{8.800704e+02, 0.000000e+00, 6.635881e+02 }, {0.000000e+00, 8.798504e+02, 2.690108e+02}, {0.000000e+00, 0.000000e+00, 1.000000e+00}};
    double d1[1][5] = {{ -3.695739e-01, 1.726456e-01, -1.281525e-03, 1.188796e-03, -4.284730e-02}};
    double d2[1][5] = {{-3.753454e-01, 1.843265e-01, -1.307069e-03, 2.190397e-03, -4.989103e-02}};

    Mat CM1 (3,3, CV_64FC1, cm1);
    Mat CM2 (3,3, CV_64FC1, cm2);
    Mat D1(1,5, CV_64FC1, d1);
    Mat D2(1,5, CV_64FC1, d2);

    double r[3][3] = {{9.998381e-01, 1.610234e-02, 8.033237e-03},{-1.588968e-02, 9.995390e-01, -2.586908e-02 },{-8.446087e-03, 2.573724e-02, 9.996331e-01}};
    double t[3][4] = {{ -5.706425e-01}, {8.447320e-03}, {1.235975e-02}};

    Mat R (3,3, CV_64FC1, r);
    Mat T (3,1, CV_64FC1, t);

    //Mat   R, T;
    Mat R1, R2, T1, T2, Q, P1, P2;

    stereoRectify(CM1, D1, CM2, D2, img1.size(), R, T, R1, R2, P1, P2, Q);

    Mat points, points1;
    reprojectImageTo3D(disp8, points, Q, true);


    imshow("points", points);
    cvtColor(points, points1, CV_BGR2GRAY);
    imshow("points1", points1);

    imwrite("disparity.jpg", disp8);
    imwrite("points1.jpg", points1);
    imwrite("points.jpg", points);


    for(int i=0; i<points.rows; ++i)
    {
        Point3f* point = points.ptr<Point3f>(i) ;
        for(int j=0; j<points.cols; ++j)
        {
            out<<i<<" "<<j<<"  x: "<<(*point).x<<" y: "<<(*point).y<<" z: "<<(*point).z<<endl;
            ++point;
        }
    }

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_xyzrgb (new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz (new pcl::PointCloud<pcl::PointXYZ>);

    for (int rows = 0; rows < points.rows; ++rows) {
        for (int cols = 0; cols < points.cols; ++cols) {
            cv::Point3f point = points.at<cv::Point3f>(rows, cols);


            pcl::PointXYZ pcl_point(point.x, point.y, point.z); // normal PointCloud
            pcl::PointXYZRGB pcl_point_rgb;
            pcl_point_rgb.x = point.x;    // rgb PointCloud
            pcl_point_rgb.y = point.y;
            pcl_point_rgb.z = point.z;
            cv::Vec3b intensity = img1.at<cv::Vec3b>(rows,cols); //BGR
            uint32_t rgb = (static_cast<uint32_t>(intensity[2]) << 16 | static_cast<uint32_t>(intensity[1]) << 8 | static_cast<uint32_t>(intensity[0]));
            pcl_point_rgb.rgb = *reinterpret_cast<float*>(&rgb);

            cloud_xyz->push_back(pcl_point);
            cloud_xyzrgb->push_back(pcl_point_rgb);
        }
    }

    visualization::CloudViewer viewer("Simple Cloud Viewer");
    viewer.showCloud(cloud_xyzrgb);

    waitKey(0);

    return 0;
}
