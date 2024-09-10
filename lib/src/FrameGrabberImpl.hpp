#pragma once

#include <qframgrabber/Buffer.hpp>
#include <qframgrabber/Types.hpp>
#include <SharedCvMat/SharedCapture.hpp>

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio/videoio.hpp>

#include <atomic>
#include <thread>

namespace frame_grabber::impl
{

class FrameGrabberImpl final
{
public:
    using BufferT = std::shared_ptr<threads::utils::Buffer<cv::Mat>>;

public:
    explicit FrameGrabberImpl(InputInfo inputInfo);
    explicit FrameGrabberImpl();
    ~FrameGrabberImpl();

    void SetBuffer(BufferT buffer);

    bool Start();
    bool Start(InputInfo&& inputInfo);
    void Stop();

    [[nodiscard]] bool IsConnected() const;

private:
    void Worker();

    bool Reconnect();

private:
    using CaptureT = std::variant<std::monostate, cv::VideoCapture, shared_cv_mat::SharedCapture>;

    CaptureT _videoCapture;
    InputInfo _inputInfo;
    BufferT _buffer;
    int32_t _badFrameCounter { 0 };

    std::atomic_bool _isWorking { false };
    std::thread _workTh;

    // TODO: std function for failed reconnect
};

} // namespace frame_grabber::impl
