#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>

using namespace std;

class bounded_buffer : private boost::noncopyable
{
public:
    typedef boost::mutex::scoped_lock lock;
    bounded_buffer(int n) : begin(0), end(0), buffered(0), circular_buf(n) { }
    void send (cv::Mat & m) {
        lock lk(monitor);
        while (buffered == circular_buf.size())
            buffer_not_full.wait(lk);
        circular_buf[end] = m;
        end = (end+1) % circular_buf.size();
        ++buffered;
        buffer_not_empty.notify_one();
        std::cout << "sent frame:buffered = " << buffered << std::endl;
    }
    cv::Mat receive() {
        lock lk(monitor);
        while (buffered == 0)
            buffer_not_empty.wait(lk);
        cv::Mat i = circular_buf[begin];
        begin = (begin+1) % circular_buf.size();
        --buffered;
        buffer_not_full.notify_one();
        std::cout << "got frame:buffered = " << buffered << std::endl;
        return i;
    }
private:
    int begin, end, buffered;
    std::vector<cv::Mat> circular_buf;
    boost::condition buffer_not_full, buffer_not_empty;
    boost::mutex monitor;
};

static bounded_buffer g_to_process_frame_buffer(4);
static bounded_buffer g_src_frame_buffer(4);
static bounded_buffer g_processed_frame_buffer(4);
static cv::VideoCapture g_capture;
static cv::VideoWriter g_writer;
static volatile bool g_is_running = true;

void capture_frame(){

    while(g_is_running){
        cv::Mat frame;
        g_capture >> frame;
        if(frame.empty()){
            std::cerr << "captured empty frame\n";
            continue;
        }
        g_writer.write(frame);
        cv::Mat to_processed_frame = frame.clone();
        g_src_frame_buffer.send(frame);
        g_to_process_frame_buffer.send(to_processed_frame);

    }
    g_capture.release();
    g_writer.release();
    std::cout << "finished capturing \n";
}

void processing_frame(){
    while(g_is_running){
        cv::Mat frame = g_to_process_frame_buffer.receive();
        if(frame.empty()){
            continue;
        }
        cv::Mat dist;
        cv::cvtColor(frame,dist,cv::COLOR_RGB2GRAY);
        cv::Canny(dist,dist,80,150);
        g_processed_frame_buffer.send(dist);
    }
    std::cout << "finished processing frame" << std::endl;
}

void display_src_frame(){
    cv::Mat frame = g_src_frame_buffer.receive();
    if(!frame.empty()){
       cv::imshow("src",frame);
    }

}

void display_processed_frame(){
    cv::namedWindow("processed_frame");
    cv::Mat frame = g_processed_frame_buffer.receive();
    if(!frame.empty()){
        cv::imshow("processed_frame",frame);
    }
}


int main(int argc, char *argv[])
{
    g_capture = cv::VideoCapture(0);
    if(!g_capture.isOpened()){
        std::cerr << "can not open camera" << std::endl;
        g_is_running = false;
        return 0;
    }
    g_writer = cv::VideoWriter("out.mp4",cv::VideoWriter::fourcc('P','I','M','2'),15,cv::Size(640,480),true);
    cv::namedWindow("src");
    cv::namedWindow("processed_frame");
    char key = -1;
    boost::thread capture_thread(&capture_frame);
    boost::thread processing_frame_thread(&processing_frame);

    while(true){
        display_src_frame();
        display_processed_frame();
        key = cv::waitKey(10);
        if(key == 27){
            g_is_running = false;
            break;
        }
    }

    capture_thread.join();
    processing_frame_thread.join();
    cv::destroyAllWindows();

    return 0;
}
