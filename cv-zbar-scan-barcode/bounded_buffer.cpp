#include "bounded_buffer.h"

namespace kkoolerter{

void bounded_buffer::send (cv::Mat & m) {
    lock lk(monitor);
    while (buffered == circular_buf.size())
        buffer_not_full.wait(lk);
    circular_buf[end] = m;
    end = (end+1) % circular_buf.size();
    ++buffered;
    buffer_not_empty.notify_one();
}


cv::Mat bounded_buffer::receive() {
    lock lk(monitor);
    while (buffered == 0)
        buffer_not_empty.wait(lk);
    cv::Mat i = circular_buf[begin];
    begin = (begin+1) % circular_buf.size();
    --buffered;
    buffer_not_full.notify_one();
    return i;
}

}
