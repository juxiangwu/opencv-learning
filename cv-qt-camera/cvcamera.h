#ifndef CVCAMERA_H
#define CVCAMERA_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

class CVCamera : public QObject
{
    Q_OBJECT
public:
    explicit CVCamera(QObject *parent = 0,int camera = 0);
    ~CVCamera();
    void resize(int width, int height);

signals:
    void sendFrame(const QImage &frame);

public slots:
    bool open();
    void close();
    bool isOpened() const;
    QString takePhoto();

private slots:
    void readFrame();
    void releaseCamara();

private:
    int m_width;
    int m_height;
    bool m_opened;
    int m_camera_index;
    cv::VideoCapture m_camera;

};

#endif // CVCAMERA_H
