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

FrameGrabberImpl::FrameGrabberImpl() = default;

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
    if (!_isWorking)
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

bool FrameGrabberImpl::IsConnected() const
{
    try
    {
        return std::visit(
            [this](const auto& cap) -> bool
            {
                if constexpr (!std::is_same_v<std::decay_t<decltype(cap)>, std::monostate>)
                    return cap.isOpened();

                return false;
            },
            _videoCapture);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return false;
}

void FrameGrabberImpl::Worker()
{
    cv::Mat frame;
    auto reader = [this, &frame](auto& cap) -> bool
    {
        if constexpr (!std::is_same_v<std::decay_t<decltype(cap)>, std::monostate>)
            return cap.read(frame);

        return false;
    };
    // TODO: add to settings
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;tcp|fflags;nobuffer|flags;low_delay", 1);
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
            if (!IsConnected()) [[unlikely]]
                break;

            try
            {
                if (std::visit(reader, _videoCapture)) // cpp20[[likely]]
                {
                    _badFrameCounter = maxBadFrames;
                    _buffer->Emplace(std::move(frame));
                }
                else
                {
                    --_badFrameCounter;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }
}

bool FrameGrabberImpl::Reconnect()
{
    try
    {
        return std::visit(
            [this](const auto& input) -> bool
            {
                std::visit(
                    [this](auto& cap) -> void
                    {
                        if constexpr (!std::is_same_v<std::decay_t<decltype(cap)>, std::monostate>)
                            cap.release();
                    },
                    _videoCapture);

                if constexpr (std::is_same_v<std::decay_t<decltype(input)>, Url>)
                {
                    _videoCapture.emplace<cv::VideoCapture>();
                    if (std::get<cv::VideoCapture>(_videoCapture).open(input.url))
                        return true;

                    std::cerr << "FrameGrabberImpl::Reconnect unable to open: " << input.url << std::endl;
                }

                if constexpr (std::is_same_v<std::decay_t<decltype(input)>, int32_t>)
                {
                    _videoCapture.emplace<cv::VideoCapture>();
                    if (std::get<cv::VideoCapture>(_videoCapture).open(input))
                        return true;

                    std::cerr << "FrameGrabberImpl::Reconnect cv::VideoCapture unable to open: " << input << std::endl;
                }

                if constexpr (std::is_same_v<std::decay_t<decltype(input)>, SharedMat>)
                {
                    _videoCapture.emplace<shared_cv_mat::SharedCapture>(1000);
                    if (std::get<shared_cv_mat::SharedCapture>(_videoCapture).open(input.sharedMemory))
                        return true;

                    std::cerr << "FrameGrabberImpl::Reconnect shared_cv_mat::SharedCapture unable to open : "
                              << input.sharedMemory << std::endl;
                }

                return false;
            },
            _inputInfo);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return false;
}
} // namespace frame_grabber::impl

// frame_grabber