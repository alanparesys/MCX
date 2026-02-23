#include "mcx/console.hpp"
#include "mcx/log.hpp"
#include "mcx/command_parser.hpp"

#include <iostream>
#include <thread>

namespace mcx {

void Console::Start() {
    running_ = true;
    thread_ = std::thread(&Console::ReadLoop, this);
}

void Console::Stop() {
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

void Console::OnCommand(std::function<void(const ConsoleCommand&)> handler) {
    handler_ = std::move(handler);
}

void Console::ReadLoop() {
    std::string line;
    while (running_ && std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        auto parsed = ParseCommand(line);
        if (handler_) {
            handler_({parsed.name, parsed.args});
        }
    }
}

} // namespace mcx
