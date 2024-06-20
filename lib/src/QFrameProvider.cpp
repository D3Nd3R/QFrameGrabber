#include "FrameGrabberImpl.hpp"

#include <qframgrabber/QFrameProvider.hpp>

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
        emit SendFrame(*frame);
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
}

} // namespace frame_grabber