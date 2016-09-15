#include "cvcamera.h"
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QDir>

#include <QDebug>

CVCamera::CVCamera(QObject *parent, int camera) : QObject(parent), m_opened(false),m_camera_index(camera)

{
    // m_camera = NULL;
    m_width = 640;
    m_height = 480;
}

CVCamera::~CVCamera()
{
    if(m_camera.isOpened()){
        m_opened = false;
        m_camera.release();
    }
}

void CVCamera::resize(int width, int height)
{
    width <= 0 ? width = 1 : width;
    height <= 0 ? height = 1 : height;
    m_width = width;
    m_height = height;
}


bool CVCamera::open()
{
    if( m_opened ){
        return m_opened;
    }
    //return m_opened = true;

    m_camera = cv::VideoCapture(0);
    m_opened = m_camera.isOpened();
    if(m_opened)
        QTimer::singleShot(0, this, SLOT(readFrame()));

    return m_opened;
}

void CVCamera::close()
{
    m_opened = false;
    m_camera.release();
}

bool CVCamera::isOpened() const
{
    return m_opened;
}

QString CVCamera::takePhoto()
{
    QString d = QDate::currentDate().toString("yyyy-MM-dd ");
    QString t =  QTime::currentTime().toString("hhmmss");
    QString photoName = QString("photo%1.jpg").arg(d+t);

    QString savePath = QDir::currentPath() + "/photos/";
    QDir dir(savePath);
    if( !dir.exists(savePath) )
        dir.mkdir(savePath);

    photoName = savePath + photoName;

    //IplImage *srcFrame = cvQueryFrame(m_camera);
    //cvCvtColor(srcFrame, srcFrame, CV_BGR2RGB);
    cv::Mat srcFrame;
    m_camera >> srcFrame;
    while(srcFrame.empty()){
        m_camera >> srcFrame;
    }
    //cv::cvtColor(srcFrame,srcFrame,cv::COLOR_BGR2RGB);
    cv::imwrite(photoName.toStdString().data(),srcFrame);
    return photoName;
}


void CVCamera::readFrame()
{
    if( !m_camera.isOpened() ) return;
    cv::Mat srcFrame;
    while( m_opened )
    {


        m_camera >> srcFrame;
        if(srcFrame.empty()){
            continue;
        }
        cv::cvtColor(srcFrame,srcFrame,cv::COLOR_BGR2RGB);
        QImage image(srcFrame.data, srcFrame.cols,
                     srcFrame.rows, QImage::Format_RGB888);
        emit sendFrame(image);
    }

    m_camera.release();
    m_opened = false;
}

void CVCamera::releaseCamara()
{
    if(m_camera.isOpened()){
        m_camera.release();
        m_opened = false;
    }
}
