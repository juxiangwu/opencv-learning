#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QImage>
#include <QTimer>
#include <QThread>

class CVCamera;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openCamara();
    void closeCamara();
    void recvFrame(const QImage &frame);

    void takePhoto();
    void clearViewr();
    void addPhotoToList(const QPixmap &photo, const QString &name);
    bool savePhoto(const QImage &image, const QString &basename);
    bool openPhoto(QListWidgetItem *item);

protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    void updateUI();

private:
    Ui::MainWindow *ui;
    QThread     m_thread;
    CVCamera*   m_camera;
    int         m_index;
};


#endif // MAINWINDOW_H
