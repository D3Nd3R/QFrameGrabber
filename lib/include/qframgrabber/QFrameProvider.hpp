#pragma once
#include "Types.hpp"

#include <qframgrabber/Buffer.hpp>

#include <opencv2/core/core.hpp>

#include <QThread>
#include <QImage>

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

    [[nodiscard]] bool IsWorking() const noexcept;

    bool Start(InputInfo&& inputInfo);

public:
    Q_PROPERTY(bool isSendQImage MEMBER _isSendQImage READ IsSendQImage WRITE SetSendQImage)

    [[nodiscard]] bool IsSendQImage() const;
    void SetSendQImage(bool newIsSendQImage);

    Q_PROPERTY(bool isSendCvMat MEMBER _isSendCvMat READ IsSendCvMat WRITE SetSendCvMat)

    [[nodiscard]] bool IsSendCvMat() const;
    void SetSendCvMat(bool newIsSendCvMat);

signals:
    void SendFrame(cv::Mat);
    void SendImage(QImage);

private:
    using BufferT = std::shared_ptr<threads::utils::Buffer<cv::Mat>>;

    void run() override;

private:
    std::unique_ptr<impl::FrameGrabberImpl> _frameGrabber { nullptr };
    std::atomic_bool _isWorking { false };
    BufferT _buffer;
    bool _isSendQImage{false};
    bool _isSendCvMat{false};
};



} // namespace frame_grabber
