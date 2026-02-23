#pragma once

#include <functional>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

namespace mcx {

using Task = std::function<void()>;

struct TimedTask {
    std::chrono::steady_clock::time_point when;
    Task what;
    
    bool operator>(const TimedTask& other) const {
        return when > other.when;
    }
};

class ThreadScheduler {
public:
    ThreadScheduler();
    ~ThreadScheduler();
    
    void Schedule(std::chrono::milliseconds delay, Task task);
    void Stop();
    
private:
    void WorkerThread();
    
    std::priority_queue<TimedTask, std::vector<TimedTask>, std::greater<>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread worker_;
    std::atomic<bool> running_{true};
};

} // namespace mcx
