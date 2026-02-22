#pragma once

#include <chrono>
#include <functional>
#include <vector>

namespace mcx {

using Task = std::function<void()>;
using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;

struct ScheduledTask {
    TimePoint when;
    Task what;
};

class Scheduler {
public:
    void Schedule(std::chrono::milliseconds delay, Task task);
    void Update();
    void Clear();

private:
    std::vector<ScheduledTask> tasks_;
};

} // namespace mcx
