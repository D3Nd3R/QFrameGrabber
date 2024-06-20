#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace frame_grabber::threads::utils
{
template<class T>
class BasicQueue final
{
public:
    template<class... Args>
    void Emplace(Args&&... args)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.emplace(std::forward<Args>(args)...);
        }
        _cv.notify_all();
    }

    bool IsEmpty() const noexcept
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _queue.empty();
    }

    template<class Rep, class Period>
    std::optional<T> WaitPop(const std::chrono::duration<Rep, Period>& waitTime = std::chrono::milliseconds { 1 })
    {
        std::optional<T> result;
        std::unique_lock lock { _mutex };
        if (_cv.wait_for(lock, waitTime, [this] { return !_queue.empty(); }))
        {
            result = std::move(_queue.front());
            _queue.pop();
        }
        return result;
    }

private:
    std::queue<T> _queue;
    mutable std::mutex _mutex;
    std::condition_variable _cv;
};

template<class T>
using Buffer = BasicQueue<T>;
} // namespace videostreamer::threads::utils