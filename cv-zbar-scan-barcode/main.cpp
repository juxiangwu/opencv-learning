#include <iostream>
#include <opencv2/opencv.hpp>

#include <zbar.h>
#include "bounded_buffer.h"

using namespace std;

kkoolerter::bounded_buffer g_frame_buffer(4);
volatile bool g_is_running = true;
zbar::ImageScanner g_barcode_scaner;

void scan_barcode_func(){
    while(g_is_running){
        cv::Mat qr_frame_image = g_frame_buffer.receive();
        if(qr_frame_image.empty()){
            continue;
        }
        int width = qr_frame_image.cols;
        int height = qr_frame_image.rows;
        zbar::Image qrimage(width,height,"Y800",qr_frame_image.data,width * height);
        g_barcode_scaner.scan(qrimage);

        for(zbar::Image::SymbolIterator symbol = qrimage.symbol_begin();
            symbol != qrimage.symbol_end();
            ++symbol) {
            // do something useful with results
            std::cout << "decoded " << symbol->get_type_name()
                 << " symbol \"" << symbol->get_data() << '"' << std::endl;
        }
        // clean up
        qrimage.set_data(NULL, 0);
    }
}

int main(int argc, char *argv[])
{
    g_barcode_scaner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cerr << "cannot open camera\n";
        g_is_running = false;
        return 0;
    }
    char key = -1;
    cv::Mat frame;
    cv::namedWindow("src");
    cv::namedWindow("qr-image");
    cv::Rect rect(270,120,258,258);
    cv::Rect copy_rect(271,121,256,256);
    boost::thread scan_thread(&scan_barcode_func);
    while(true){
        cap >> frame;
        if(frame.empty()){
            continue;
        }

        cv::rectangle(frame,rect,cv::Scalar(0,0,255),1);
        cv::Mat qr_frame_image(frame,copy_rect);
        cv::cvtColor(qr_frame_image,qr_frame_image,cv::COLOR_RGB2GRAY);
        g_frame_buffer.send(qr_frame_image);
        cv::imshow("src",frame);
        cv::imshow("qr-image",qr_frame_image);
        key = cv::waitKey(10);
        if(key == 27){
            g_is_running = false;
            break;
        }
    }
    scan_thread.join();
    return 0;
}
