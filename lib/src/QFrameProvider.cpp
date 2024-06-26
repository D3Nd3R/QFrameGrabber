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
            continue;

        if (IsSendCvMat())
            emit SendFrame(*frame);

        if (IsSendQImage())
        {
            cv::cvtColor(*frame, *frame, cv::COLOR_BGR2RGB);
            emit SendImage(frame_grabber::utils::CvMat2QImage(*frame));
        }
    }
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

    _frameGrabber->SetBuffer(_buffer);
    _isWorking = true;
    if (_frameGrabber->Start(std::move(inputInfo)))
        start();

    return false;
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