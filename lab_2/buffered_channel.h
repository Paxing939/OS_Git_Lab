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

    }

    std::pair<T, bool> Recv() {

    }

    void Close() {

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