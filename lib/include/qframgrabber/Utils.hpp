#pragma once

#include <opencv2/core/mat.hpp>

#include <QImage>

namespace frame_grabber::utils
{
/// Create QImage from cv::Mat
/// \details QImage use buffer managed by cv::Mat
/// @param mat Input image
/// @return QImage with buffer allocated by mat
QImage CvMat2QImage(const cv::Mat& mat);
QImage DeepCopy(const QImage& image);
} // namespace frame_grabber::utils
