//#include <XnCppWrapper.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pcl/visualization/cloud_viewer.h>

void matToPointXYZ(cv::Mat &color, cv::Mat &depth,
                   pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud) {
    const double u0 = 3.3930780975300314e+02;
    const double v0 = 2.4273913761751615e+02;
    const double fx = 5.9421434211923247e+02;
    const double fy = 5.9104053696870778e+02;
    int rows = color.rows;
    int cols = color.cols;
    cloud->height = (uint32_t) rows;
    cloud->width = (uint32_t) cols;
    cloud->is_dense = false;
    cloud->points.resize(cloud->width * cloud->height);
    for (unsigned int u = 0; u < rows; ++u) {
        for (unsigned int v = 0; v < cols; ++v) {
            float Xw = 0, Yw = 0, Zw = 0;

            Zw = depth.at<ushort>(u, v);
            Xw = (float) ((v - v0) * Zw / fx);
            Yw = (float) ((u - u0) * Zw / fy);

            cloud->at(v, u).b = color.at<cv::Vec3b>(u, v)[0];
            cloud->at(v, u).g = color.at<cv::Vec3b>(u, v)[1];
            cloud->at(v, u).r = color.at<cv::Vec3b>(u, v)[2];
            cloud->at(v, u).x = Xw;
            cloud->at(v, u).y = Yw;
            cloud->at(v, u).z = Zw;
        }
    }
}

pcl::PointCloud<pcl::PointXYZ>::Ptr MatToPointXYZ(cv::Mat OpencVPointCloud)
{
    /*
             *  Function: Get from a Mat to pcl pointcloud datatype
             *  In: cv::Mat
             *  Out: pcl::PointCloud
             */

    //char pr=100, pg=100, pb=100;
    pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);//(new pcl::pointcloud<pcl::pointXYZ>);

    for(int i=0;i<OpencVPointCloud.cols;i++)
    {
        //std::cout<<i<<endl;

        pcl::PointXYZ point;
        point.x = OpencVPointCloud.at<float>(0,i);
        point.y = OpencVPointCloud.at<float>(1,i);
        point.z = OpencVPointCloud.at<float>(2,i);

        // when color needs to be added:
        //uint32_t rgb = (static_cast<uint32_t>(pr) << 16 | static_cast<uint32_t>(pg) << 8 | static_cast<uint32_t>(pb));
        //point.rgb = *reinterpret_cast<float*>(&rgb);

        point_cloud_ptr -> points.push_back(point);


    }
    point_cloud_ptr->width = (int)point_cloud_ptr->points.size();
    point_cloud_ptr->height = 1;

    return point_cloud_ptr;

}

int user_data;

void viewerOneOff (pcl::visualization::PCLVisualizer& viewer)
{
    viewer.setBackgroundColor (0.0, 0.0, 0.0);
}

void viewerPsycho (pcl::visualization::PCLVisualizer& viewer)
{
    user_data = 9;
}

int main(int argc, char **argv) {
    //  xn::Context xContext;
    //  xContext.Init();
    //  xn::DepthGenerator xDepth;
    //  xDepth.Create(xContext);
    //  xn::ImageGenerator xImage;
    //  xImage.Create(xContext);
//    xDepth.GetAlternativeViewPointCap().SetViewPoint(xImage);
//    cv::namedWindow("Depth Image", CV_WINDOW_AUTOSIZE);
//    cv::namedWindow("Color Image", CV_WINDOW_AUTOSIZE);
//    //  xContext.StartGeneratingAll();

//    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
//    pcl::visualization::CloudViewer viewer("Viewer");

//    while (1) {
//        xContext.WaitAndUpdateAll();
//        xn::ImageMetaData xColorData;
//        xImage.GetMetaData(xColorData);

//        cv::Mat cColorImg(xColorData.FullYRes(), xColorData.FullXRes(), CV_8UC3, (void *) xColorData.Data());
//        cv::Mat cBGRImg;
//        cv::cvtColor(cColorImg, cBGRImg, CV_RGB2BGR);
//        cv::imshow("Color Image", cBGRImg);

//        xn::DepthMetaData xDepthData;
//        xDepth.GetMetaData(xDepthData);

//        cv::Mat cDepthImg(xDepthData.FullYRes(), xDepthData.FullXRes(), CV_16UC1, (void *) xDepthData.Data());
//        cv::Mat c8BitDepth;
//        cDepthImg.convertTo(c8BitDepth, CV_8U, 255.0 / 7000);
//        cv::imshow("Depth Image", c8BitDepth);

//        matToPointXYZ(cBGRImg, cDepthImg, cloud);
//        viewer.showCloud(cloud);

//        if (cv::waitKey(10) >= 0) break;
//    }

    cv::Mat image = cv::imread("/home/jenson/Pictures/f1.jpg");
    cv::Mat depth;
    image.convertTo(depth,CV_32F);
    // pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = MatToPointXYZ(image);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
  //  pcl::visualization::CloudViewer viewer("Cloud Viewer");
    pcl::visualization::CloudViewer viewer("Cloud Viewer");

    matToPointXYZ(image,depth,cloud);
    viewer.showCloud(cloud);

    viewer.runOnVisualizationThreadOnce (viewerOneOff);

    viewer.runOnVisualizationThread (viewerPsycho);

    while (!viewer.wasStopped ())
    {
        user_data = 9;
    }

    return 0;
}
