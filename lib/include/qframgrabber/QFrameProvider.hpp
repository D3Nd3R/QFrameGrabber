#pragma once
#include "Types.hpp"

#include <qframgrabber/Buffer.hpp>

#include <opencv2/core/core.hpp>

#include <QThread>

namespace frame_grabber
{
namespace impl
{
class FrameGrabberImpl;
}

class QFrameProvider final : public QThread
{
    Q_OBJECT
public:
    explicit QFrameProvider(InputInfo&& inputInfo);
    ~QFrameProvider() override;
    bool Start();
    void Stop();

    bool IsWorking() const noexcept;

    bool Start(InputInfo&& inputInfo);

signals:
    void SendFrame(cv::Mat);

private:
    using BufferT = std::shared_ptr<frame_grabber::threads::utils::Buffer<cv::Mat>>;

    void run() override;

private:
    std::unique_ptr<impl::FrameGrabberImpl> _frameGrabber { nullptr };
    std::atomic_bool _isWorking { false };
    BufferT _buffer;
};

} // namespace frame_grabber
