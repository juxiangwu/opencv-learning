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

static volatile bool g_is_running = true;

static bounded_buffer g_capture_0_buffer(4);
static bounded_buffer g_capture_1_buffer(4);
static cv::VideoCapture g_capture_0;
static cv::VideoCapture g_capture_1;

void capture_fun_1(){
    g_capture_0 = cv::VideoCapture(0);
    if(!g_capture_0.isOpened()){
        std::cerr << "can not open camera 1 \n";
        return ;
    }
    while(g_is_running && g_capture_0.isOpened()){
        std::cout << "capture_fun_1:still capture \n";
        cv::Mat frame;
        g_capture_0 >> frame;
        if(frame.empty()){
            continue;
        }
        g_capture_0_buffer.send(frame);
    }

    //g_capture_0.release();
    std::cout << "camera 0 released\n";
}

void display_capture_1(){
    cv::namedWindow("Video:0");
    cv::Mat frame = g_capture_0_buffer.receive();
    if(!frame.empty()){
        cv::imshow("Video:0",frame);
    }
}

void capture_fun_2(){
    g_capture_1 = cv::VideoCapture(1);
    if(!g_capture_1.isOpened()){
        std::cerr <<"can not open camera 0 \n";
        return;
    }
    std::cout << "camera 0 started \n";
    while(g_is_running && g_capture_1.isOpened()){
        std::cout << "capture_fun_2:still capture \n";
        cv::Mat frame;
        g_capture_1 >> frame;
        if(frame.empty()){
            continue;
        }
        g_capture_1_buffer.send(frame);
    }

    //g_capture_1.release();
    std::cout << "camera 1 released\n";
}

void display_capture_2(){

    cv::namedWindow("Video:1");
    cv::Mat frame = g_capture_1_buffer.receive();
    if(!frame.empty()){
        cv::imshow("Video:1",frame);
    }
}

int main(int argc, char *argv[])
{


    std::cout << "camera 1 started \n";

    boost::thread capture_thread_1(&capture_fun_1);
    boost::thread capture_thread_2(&capture_fun_2);


    char key = -1;

    while(true){
        display_capture_1();
        display_capture_2();
        key = cv::waitKey(10);
        if(key == 27){
            g_is_running = false;
            break;
        }
    }
    std::cout << "finished \n";
    g_is_running = false;

    g_capture_0.release();
    g_capture_1.release();

    capture_thread_1.join();
    capture_thread_2.join();
     cv::destroyAllWindows();
    std::cout << "all finished \n";
    return 0;
}
