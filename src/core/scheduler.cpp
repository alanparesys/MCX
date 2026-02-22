#include "mcx/scheduler.hpp"

#include <algorithm>

namespace mcx {

void Scheduler::Schedule(std::chrono::milliseconds delay, Task task) {
    tasks_.push_back({Clock::now() + delay, std::move(task)});
}

void Scheduler::Update() {
    const auto now = Clock::now();
    auto it = tasks_.begin();
    while (it != tasks_.end()) {
        if (it->when <= now) {
            it->what();
            it = tasks_.erase(it);
        } else {
            ++it;
        }
    }
}

void Scheduler::Clear() {
    tasks_.clear();
}

} // namespace mcx
