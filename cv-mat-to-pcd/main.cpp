#include <pcl/io/pcd_io.h>
#include <pcl/console/time.h>
#include <pcl/console/print.h>
#include <pcl/console/parse.h>
#include <pcl/io/vtk_lib_io.h>
#include <opencv2/opencv.hpp>
#include <vtk/vtkImporter.h>
#include <vtk/vtkImageImport.h>
#include <pcl/visualization/cloud_viewer.h>

#define RED_MULTIPLIER 0.299
#define GREEN_MULTIPLIER 0.587
#define BLUE_MULTIPLIER 0.114
#define MAX_COLOR_INTENSITY 255

using namespace pcl;
using namespace pcl::io;
using namespace pcl::console;
using namespace cv;

void mat2vtkImageData(const Mat &source,vtkSmartPointer<vtkImageData> dst)
{
//    vtkSmartPointer<vtkImageData> output =   vtkSmartPointer<vtkImageData>( vtkImageData::New());
//    //vtkSmartPointer<vtkInformation> outInfo = vtkSmartPointer<vtkInformation>::New();

//    output->SetDimensions(source.cols, source.rows, 1);
//    output->AllocateScalars(VTK_UNSIGNED_CHAR, source.channels());

//    unsigned char* dptr = reinterpret_cast<unsigned char*>(output->GetScalarPointer());
//    size_t elem_step = output->GetIncrements()[1]/sizeof(unsigned char);

//    for (int y = 0; y < source.rows; ++y)
//    {
//        unsigned char* drow = dptr + elem_step * y;
//        const unsigned char *srow = source.ptr<unsigned char>(y);
//        for (int x = 0; x < source.cols; ++x)
//            drow[x] = *srow++;
//    }

//    output->Modified();
//    return output;
      vtkImageImport *importer = vtkImageImport::New();

      importer->SetOutput( dst.Get() );
      importer->SetDataSpacing( 1, 1, 1 );
      importer->SetDataOrigin( 0, 0, 0 );
      importer->SetWholeExtent(   0, source.size().width-1, 0, source.size().height-1, 0, 0 );
      importer->SetDataExtentToWholeExtent();
      importer->SetDataScalarTypeToUnsignedChar();
      importer->SetNumberOfScalarComponents( source.channels() );
      importer->SetImportVoidPointer( source.data );
      importer->Update();

}

inline void depth2xyz (float v_viewing_angle, float h_viewing_angle,
           int image_width, int image_height, int image_x, int image_y,
           float depth, float &x, float &y, float &z)
{
    float width, height;
    static const float PI = 3.1415927;

    if (depth <= 0.0f)
    {
        x = y = z = std::numeric_limits<float>::quiet_NaN ();
    }
    else
    {
        width = depth * std::tan (h_viewing_angle * PI / 180 / 2) * 2;
        height = depth * std::tan (v_viewing_angle * PI / 180 / 2) * 2;

        x = (image_x - image_width / 2.0) / image_width * width;
        y = (image_height / 2.0 - image_y) / image_height * height;
        z = depth;
    }
}

void mat2PointXYZRGB(const cv::Mat & src,pcl::PointCloud<PointXYZRGB>::Ptr rgb_depth_cloud){
    //vtkSmartPointer<vtkImageData> color_image_data = mat2vtkImageData(src);
    vtkSmartPointer<vtkImageData> color_image_data =   vtkSmartPointer<vtkImageData>( vtkImageData::New());
    mat2vtkImageData(src,color_image_data);
    int dimensions[3];
    color_image_data->GetDimensions (dimensions);
//   cv::Mat depth_image_data_gray,depth_image_data_thresh;
//    cv::cvtColor(src,depth_image_data_gray,cv::COLOR_RGB2GRAY);
//    cv::threshold(depth_image_data_gray,depth_image_data_thresh,10,80,cv::THRESH_BINARY);
//    vtkSmartPointer<vtkImageData> depth_image_data = mat2vtkImageData(depth_image_data_thresh);
    double pixel[3] = {0,0,0};
    float v_viewing_angle = 90.0f;
    float h_viewing_angle = 90.0f;
    float depth_unit_magic = 1000.0f;
    rgb_depth_cloud->width = dimensions[0];
    rgb_depth_cloud->height = dimensions[1]; // This indicates that the point cloud is organized
    rgb_depth_cloud->is_dense = false;
    rgb_depth_cloud->resize (rgb_depth_cloud->width * rgb_depth_cloud->height);
    float depth = 1.0f;
    for (int y = 0; y < dimensions[1]; y++)
    {
        for (int x = 0; x < dimensions[0]; x++)
        {
            pixel[0] = color_image_data->GetScalarComponentAsDouble (x, y, 0, 0);
            pixel[1] = color_image_data->GetScalarComponentAsDouble (x, y, 0, 1);
            pixel[2] = color_image_data->GetScalarComponentAsDouble (x, y, 0, 2);
            depth = color_image_data->GetScalarComponentAsFloat (x, y, 0, 0) / depth_unit_magic;

            PointXYZRGB xyzrgb;
            depth2xyz (v_viewing_angle, h_viewing_angle,
                       rgb_depth_cloud->width, rgb_depth_cloud->height, x, y,
                       depth, xyzrgb.x, xyzrgb.y, xyzrgb.z);
            xyzrgb.r = static_cast<uint8_t> (pixel[0]);
            xyzrgb.g = static_cast<uint8_t> (pixel[1]);
            xyzrgb.b = static_cast<uint8_t> (pixel[2]);

            (*rgb_depth_cloud) (x, y) = xyzrgb;
        }
    }
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

int main(int argc,char * argv[]){
    cv::Mat src = cv::imread("/home/jenson/Pictures/linear.jpg");
    cv::Mat dst;
    cv::cvtColor(src,dst,cv::COLOR_BGR2RGB);
    pcl::PointCloud<PointXYZRGB>::Ptr cloud(new pcl::PointCloud<PointXYZRGB>());
    mat2PointXYZRGB(dst,cloud);
    pcl::visualization::CloudViewer viewer("Cloud Viewer");
    viewer.showCloud(cloud);

    viewer.runOnVisualizationThreadOnce (viewerOneOff);

    viewer.runOnVisualizationThread (viewerPsycho);

    while (!viewer.wasStopped ())
    {
        user_data = 9;
    }
    return 0;
}
