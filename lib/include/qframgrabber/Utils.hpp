#pragma once

#include <opencv2/core/mat.hpp>

#include <QImage>

namespace frame_grabber::utils
{

QImage CvMat2QImage(const cv::Mat& mat);

}
