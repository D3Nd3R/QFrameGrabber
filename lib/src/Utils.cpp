#include <qframgrabber/Utils.hpp>

#include <QDebug>

namespace frame_grabber::utils
{

QImage CvMat2QImage(const cv::Mat& mat)
{
    switch (mat.type())
    {
        case CV_8UC4:
            return { mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_ARGB32 };
        case CV_8UC3:
            return { mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888 };
        case CV_8UC1:

            return { mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8 };
        default:
            qWarning() << "frame_grabber::utils::CvMat2QImage() - cv::Mat image type not handled in switch:"
                       << mat.type();
            break;
    }

    return {};
}
} // namespace frame_grabber::utils
