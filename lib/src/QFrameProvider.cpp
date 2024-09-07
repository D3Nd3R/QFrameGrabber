#include "FrameGrabberImpl.hpp"

#include <qframgrabber/QFrameProvider.hpp>
#include <qframgrabber/Utils.hpp>

#include <opencv2/imgproc.hpp>

namespace frame_grabber
{
QFrameProvider::QFrameProvider(InputInfo&& inputInfo)
    : _frameGrabber { std::make_unique<impl::FrameGrabberImpl>(inputInfo) }
    , _buffer { std::make_shared<BufferT::element_type>() }
{
    PrepareDefaultImages();
}

QFrameProvider::~QFrameProvider()
{
    Stop();
}

void QFrameProvider::run()
{
    while (_isWorking)
    {
        auto frame = _buffer->WaitPop(std::chrono::milliseconds { 1 });
        if (!frame)
        {
            if (_frameGrabber->IsConnected())
                continue;

            frame = SendReconImg();
            if (frame->empty())
                continue;
        }

        if (IsSendCvMat())
            emit SendFrame(*frame);

        if (IsSendQImage())
        {
            cv::cvtColor(*frame, *frame, cv::COLOR_BGR2RGB);
            emit SendImage(utils::DeepCopy(utils::CvMat2QImage(*frame)));
        }
    }
}

void QFrameProvider::PrepareDefaultImages()
{
    const cv::Size defaultSize { 1920, 1080 };
    const cv::Point center { defaultSize / 2 };

    constexpr int32_t radius { 30 };
    const cv::Point shift { radius * 2 + 10, 0 };

    const cv::Scalar white { 255, 255, 255 };
    const cv::Scalar gray { 128, 128, 128 };

    for (auto& img : _defaultMat)
        img = cv::Mat::zeros(defaultSize, CV_8UC3);

    cv::circle(_defaultMat[0], center - shift, radius, white, -1, cv::LINE_AA);
    cv::circle(_defaultMat[0], center, radius, gray, -1, cv::LINE_AA);
    cv::circle(_defaultMat[0], center + shift, radius, gray, -1, cv::LINE_AA);

    cv::circle(_defaultMat[1], center - shift, radius, gray, -1, cv::LINE_AA);
    cv::circle(_defaultMat[1], center, radius, white, -1, cv::LINE_AA);
    cv::circle(_defaultMat[1], center + shift, radius, gray, -1, cv::LINE_AA);

    cv::circle(_defaultMat[2], center - shift, radius, gray, -1, cv::LINE_AA);
    cv::circle(_defaultMat[2], center, radius, gray, -1, cv::LINE_AA);
    cv::circle(_defaultMat[2], center + shift, radius, white, -1, cv::LINE_AA);
}

cv::Mat QFrameProvider::SendReconImg()
{
    cv::Mat res;

    switch (_reconCounter)
    {
        case 0:
            res = _defaultMat[0];
            ++_reconCounter;
            break;
        case 500:
            res = _defaultMat[1];
            ++_reconCounter;
            break;
        case 1000:
            res = _defaultMat[2];
            ++_reconCounter;
            break;
        case 1500:
            _reconCounter = 0;
            break;
        default:
            ++_reconCounter;
            break;
    }
    return res;
}

bool QFrameProvider::Start()
{
    if (!_frameGrabber || !_buffer)
        return false;

    _frameGrabber->SetBuffer(_buffer);
    _isWorking = true;
    if (_frameGrabber->Start())
    {
        start();
    }
    return false;
}

void QFrameProvider::Stop()
{
    if (_frameGrabber)
        _frameGrabber->Stop();

    _isWorking = false;
    this->wait();
}

bool QFrameProvider::IsWorking() const noexcept
{
    return _isWorking;
}

bool QFrameProvider::Start(InputInfo&& inputInfo)
{
    if (!_frameGrabber || !_buffer)
        return false;

    //Stop();

    _frameGrabber->SetBuffer(_buffer);
    _isWorking = true;
    if (_frameGrabber->Start(std::move(inputInfo)))
        start();

    return true;
}

bool QFrameProvider::IsSendQImage() const
{
    return _isSendQImage;
}

void QFrameProvider::SetSendQImage(const bool newIsSendQImage)
{
    _isSendQImage = newIsSendQImage;
}

bool QFrameProvider::IsSendCvMat() const
{
    return _isSendCvMat;
}

void QFrameProvider::SetSendCvMat(const bool newIsSendCvMat)
{
    _isSendCvMat = newIsSendCvMat;
}

} // namespace frame_grabber