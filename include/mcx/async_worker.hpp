#pragma once

#include "mcx/export.hpp"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

namespace mcx {

enum class TaskPriority {
    LOW,
    NORMAL,
    HIGH
};

struct AsyncTask {
    std::string id;
    std::function<void()> work;
    std::function<void()> callback;
    TaskPriority priority{TaskPriority::NORMAL};
};

class MCX_API AsyncWorker {
public:
    AsyncWorker(size_t threadCount = 2);
    ~AsyncWorker();
    
    void Start();
    void Stop();
    
    void Submit(AsyncTask task);
    void SubmitSimple(std::function<void()> work);
    
    [[nodiscard]] size_t GetPendingCount();
    [[nodiscard]] size_t GetActiveThreads();

private:
    void WorkerLoop();
    AsyncTask GetNextTask();
    
    std::vector<std::thread> workers_;
    std::queue<AsyncTask> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_{false};
    std::atomic<size_t> activeThreads_{0};
};

} // namespace mcx
