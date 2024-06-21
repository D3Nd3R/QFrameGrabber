#include "mainwindow.h"

#include "./ui_mainwindow.h"

#include <qframgrabber/Utils.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _frameProvider.Start();

    connect(&_frameProvider, &frame_grabber::QFrameProvider::SendFrame, this, &MainWindow::OnRcvFrame);
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::OnRcvFrame(cv::Mat frame)
{
    qDebug() << "MainWindow::OnRcvFrame";

    if (frame.size() == cv::Size { 0, 0 })
    {
        qWarning() << "MainWindow::OnRcvFrame. rcv empty frame";
        return;
    }

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    ui->label->setPixmap(
        QPixmap::fromImage(frame_grabber::utils::CvMat2QImage(frame)).scaled(ui->label->size(), Qt::KeepAspectRatio));
}
