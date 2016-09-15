#ifndef BOUNDED_BUFFER_H
#define BOUNDED_BUFFER_H

#include <opencv2/opencv.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

namespace kkoolerter{

class bounded_buffer : private boost::noncopyable
{
public:
    typedef boost::mutex::scoped_lock lock;
    bounded_buffer(int n) : begin(0), end(0), buffered(0), circular_buf(n) { }
    void send (cv::Mat & m);
   cv::Mat receive();
private:
    int begin, end, buffered;
    std::vector<cv::Mat> circular_buf;
    boost::condition buffer_not_full, buffer_not_empty;
    boost::mutex monitor;
};
}

#endif // BOUNDED_BUFFER_H
