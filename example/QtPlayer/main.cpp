#include "mainwindow.h"

#include <opencv4/opencv2/core/mat.hpp>

#include <QApplication>

Q_DECLARE_METATYPE(cv::Mat)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
