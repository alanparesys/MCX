#pragma once

#include "mcx/export.hpp"
#include "mcx/events.hpp"
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>

namespace mcx {

struct ConsoleCommand {
    std::string name;
    std::vector<std::string> args;
};

class MCX_API Console {
public:
    void Start();
    void Stop();
    void OnCommand(std::function<void(const ConsoleCommand&)> handler);
    
private:
    void ReadLoop();
    
    std::function<void(const ConsoleCommand&)> handler_;
    std::atomic<bool> running_{false};
    std::thread thread_;
};

} // namespace mcx
