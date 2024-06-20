#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qframgrabber/QFrameProvider.hpp>

#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui
{
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:

    void OnRcvFrame(QString);

private:
    Ui::MainWindow* ui;

    frame_grabber::QFrameProvider _frameProvider { 0 };
};
#endif // MAINWINDOW_H
