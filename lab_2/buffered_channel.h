#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <utility>
#include <condition_variable>
#include <queue>
#include <stdexcept>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size)
            : size_(size), send_(), recv_(), mutex_(), is_close_(false) {}

    void Send(T value) {
        std::unique_lock<std::mutex> lock(mutex_);

        send_.wait(lock, [this] {
           return queue_.size() < size_ || is_close_;
        });

        if (is_close_) {
            throw std::runtime_error("Can't send, chanel is closed!");
        }

        queue_.push(std::move(value));
        recv_.notify_one();
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(mutex_);

        recv_.wait(lock, [this] {
            return !queue_.empty() || is_close_;
        });

        if (is_close_ && queue_.empty()) {
            return {std::move(T()), false};
        } else {
            T top = std::move(queue_.front());
            queue_.pop();
            send_.notify_one();
            return {std::move(top), true};
        }
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mutex_);
        is_close_ = true;
        send_.notify_all();
        recv_.notify_all();
    }

private:

    std::condition_variable send_;
    std::condition_variable recv_;
    std::mutex mutex_;
    std::queue<T> queue_;
    const int size_;
    bool is_close_;
};

#endif // BUFFERED_CHANNEL_H_