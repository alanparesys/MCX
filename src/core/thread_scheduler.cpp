#include "mcx/thread_scheduler.hpp"
#include "mcx/log.hpp"

namespace mcx {

ThreadScheduler::ThreadScheduler() : worker_(&ThreadScheduler::WorkerThread, this) {}

ThreadScheduler::~ThreadScheduler() {
    Stop();
}

void ThreadScheduler::Schedule(std::chrono::milliseconds delay, Task task) {
    std::lock_guard lock(mutex_);
    tasks_.push({std::chrono::steady_clock::now() + delay, std::move(task)});
    cv_.notify_one();
}

void ThreadScheduler::Stop() {
    running_ = false;
    cv_.notify_all();
    if (worker_.joinable()) {
        worker_.join();
    }
}

void ThreadScheduler::WorkerThread() {
    while (running_) {
        std::unique_lock lock(mutex_);
        
        if (tasks_.empty()) {
            cv_.wait(lock, [this] { return !tasks_.empty() || !running_; });
            continue;
        }
        
        auto now = std::chrono::steady_clock::now();
        if (tasks_.top().when <= now) {
            auto task = std::move(tasks_.top().what);
            tasks_.pop();
            lock.unlock();
            
            try {
                task();
            } catch (const std::exception& e) {
                log::Error("Scheduled task exception: " + std::string(e.what()));
            }
        } else {
            cv_.wait_until(lock, tasks_.top().when);
        }
    }
}

} // namespace mcx
