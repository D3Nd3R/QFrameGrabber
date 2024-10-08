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

    ui->button->setText("Start");
    _frameProvider.SetSendQImage(true);

    ui->lineEdit->setText("rtsp://192.168.1.153:8561/night");

    connect(&_frameProvider, &frame_grabber::QFrameProvider::SendImage, this, &MainWindow::OnRcvImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnRcvFrame(cv::Mat frame)
{
    if (frame.size() == cv::Size { 0, 0 })
    {
        qWarning() << "MainWindow::OnRcvFrame. rcv empty frame";
        return;
    }

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    ui->label->SetPixmap(QPixmap::fromImage(frame_grabber::utils::DeepCopy(frame_grabber::utils::CvMat2QImage(frame))));
}

void MainWindow::OnRcvImage(QImage image)
{
    ui->label->SetPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_button_clicked()
{
    qDebug() << "MainWindow::on_button_clicked";

    const auto input = ui->lineEdit->text();

    if (const QRegExp re("\\d*"); re.exactMatch(input))
    {
        bool isInt { false };
        int src = input.toInt(&isInt);
        if (isInt)
            _frameProvider.Start(src);
        return;
    }

    if (input == "mat_sender")
    {
        _frameProvider.Start(frame_grabber::SharedMat { input.toStdString() });
        return;
    }
    _frameProvider.Start(frame_grabber::Url { input.toStdString() });
}
