#include "mcx/async_worker.hpp"
#include "mcx/log.hpp"

namespace mcx {

AsyncWorker::AsyncWorker(size_t threadCount) {
    workers_.reserve(threadCount);
}

AsyncWorker::~AsyncWorker() {
    Stop();
}

void AsyncWorker::Start() {
    running_ = true;
    for (size_t i = 0; i < workers_.capacity(); ++i) {
        workers_.emplace_back(&AsyncWorker::WorkerLoop, this);
    }
    log::Info("Async worker started with " + std::to_string(workers_.size()) + " threads");
}

void AsyncWorker::Stop() {
    running_ = false;
    cv_.notify_all();
    
    for (auto& thread : workers_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    workers_.clear();
}

void AsyncWorker::Submit(AsyncTask task) {
    {
        std::lock_guard lock(mutex_);
        tasks_.push(std::move(task));
    }
    cv_.notify_one();
}

void AsyncWorker::SubmitSimple(std::function<void()> work) {
    Submit(AsyncTask{"", std::move(work), nullptr, TaskPriority::NORMAL});
}

size_t AsyncWorker::GetPendingCount() const {
    std::lock_guard lock(mutex_);
    return tasks_.size();
}

size_t AsyncWorker::GetActiveThreads() const {
    return activeThreads_.load();
}

void AsyncWorker::WorkerLoop() {
    while (running_) {
        auto task = GetNextTask();
        if (!task.work) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        
        activeThreads_++;
        
        try {
            task.work();
            if (task.callback) {
                task.callback();
            }
        } catch (const std::exception& e) {
            log::Error("Async task failed: " + std::string(e.what()));
        }
        
        activeThreads_--;
    }
}

AsyncTask AsyncWorker::GetNextTask() {
    std::unique_lock lock(mutex_);
    
    if (tasks_.empty()) {
        return AsyncTask{};
    }
    
    auto task = std::move(tasks_.front());
    tasks_.pop();
    return task;
}

} // namespace mcx
