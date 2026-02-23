#pragma once

#include "mcx/export.hpp"
#include <string>
#include <thread>
#include <atomic>
#include <functional>

namespace mcx {

using ConfigChangeCallback = std::function<void()>;

class MCX_API ConfigWatcher {
public:
    explicit ConfigWatcher(std::string filepath);
    ~ConfigWatcher();

    void Start();
    void Stop();
    void OnChange(ConfigChangeCallback callback);

private:
    void WatchLoop();

    std::string filepath_;
    std::atomic<bool> running_{false};
    std::thread watcherThread_;
    ConfigChangeCallback callback_;
};

} // namespace mcx
