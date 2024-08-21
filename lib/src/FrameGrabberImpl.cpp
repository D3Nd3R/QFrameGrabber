#include "FrameGrabberImpl.hpp"

#include <qframgrabber/MetaUtils.hpp>

#include <iostream>

namespace
{

constexpr std::chrono::milliseconds reconnetcInterval { 500 };
constexpr int32_t maxBadFrames { 10 };
} // namespace

namespace frame_grabber::impl
{
FrameGrabberImpl::FrameGrabberImpl(InputInfo inputInfo)
    : _inputInfo { std::move(inputInfo) }
{
}

FrameGrabberImpl::FrameGrabberImpl() {}

bool FrameGrabberImpl::Start(InputInfo&& inputInfo)
{
    Stop();
    _inputInfo = std::move(inputInfo);
    return Start();
}

FrameGrabberImpl::~FrameGrabberImpl()
{
    Stop();
}

void FrameGrabberImpl::SetBuffer(BufferT buffer)
{
    _buffer = std::move(buffer);
}

bool FrameGrabberImpl::Start()
{
    Stop();
    if (!_buffer)
        return false;
    _isWorking = true;

    _workTh = std::thread([this] { Worker(); });

    return true;
}

void FrameGrabberImpl::Stop()
{
    _isWorking = false;
    if (_workTh.joinable())
        _workTh.join();
}

void FrameGrabberImpl::Worker()
{
    // TODO: add to settings
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS","rtsp_transport;tcp|fflags;nobuffer|flags;low_delay",1);
    while (_isWorking)
    {
        _badFrameCounter = maxBadFrames;
        if (!Reconnect())
        {
            std::this_thread::sleep_for(reconnetcInterval);
            continue;
        }
        while (_isWorking && _badFrameCounter > 0)
        {
            if (!_videoCapture.isOpened()) [[unlikely]]
                break;

            if (cv::Mat frame; _videoCapture.read(frame)) // cpp20[[likely]]
            {
                _badFrameCounter = maxBadFrames;
                _buffer->Emplace(std::move(frame));
            }
            else
            {
                --_badFrameCounter;
            }
        }
    }
}

bool FrameGrabberImpl::Reconnect()
{
    return std::visit(meta::utils::Overload { [this](const auto& input) -> bool
                                              {
                                                  _videoCapture.release();

                                                  if (_videoCapture.open(input))
                                                      return true;

                                                  std::cerr << "FrameGrabberImpl::Reconnect unable to open: " << input
                                                            << std::endl;
                                                  return false;
                                              },
                                              [](std::monostate) { return false; } },
                      _inputInfo);
}
} // namespace frame_grabber::impl

// frame_grabber