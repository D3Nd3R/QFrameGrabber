#include "mainwindow.h"

#include "./ui_mainwindow.h"

#include <opencv2/core/core.hpp>

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

void MainWindow::OnRcvFrame(cv::Mat)
{
    qDebug() << "MainWindow::OnRcvFrame";
}
