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
    virtual ~QFrameProvider();
    bool Start();
    void Stop();

signals:
    void SendFrame(QString);

private:
    using BufferT = std::shared_ptr<frame_grabber::threads::utils::Buffer<cv::Mat>>;

    void run() override;

private:
    std::unique_ptr<impl::FrameGrabberImpl> _frameGrabber { nullptr };
    std::atomic_bool _isWorking { false };
    BufferT _buffer;
};

} // namespace frame_grabber
