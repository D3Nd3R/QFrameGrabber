#include "opencv2/opencv.hpp"

#include <qframgrabber/../../src/FrameGrabberImpl.hpp>

int main()
{
    auto buffer = std::make_shared<frame_grabber::threads::utils::Buffer<cv::Mat>>();
    frame_grabber::impl::FrameGrabberImpl grabber { 0 };
    grabber.SetBuffer(buffer);
    grabber.Start();

    while (true)
    {
        auto frame = buffer->WaitPop(std::chrono::milliseconds { 1 });
        if (!frame)
            continue;

        imshow("Frame", *frame);

        if (const int c = cv::waitKey(1); c == 27)
            break;
    }

    // Closes all the frames
    cv::destroyAllWindows();

    return 0;
}