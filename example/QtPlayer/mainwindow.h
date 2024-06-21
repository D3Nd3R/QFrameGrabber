#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qframgrabber/QFrameProvider.hpp>

#include <QLayout>
#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui
{
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

public slots:

    void OnRcvFrame(cv::Mat);

    void on_button_clicked();

private:
    Ui::MainWindow* ui;

    frame_grabber::QFrameProvider _frameProvider { 0 };
};
#endif // MAINWINDOW_H
