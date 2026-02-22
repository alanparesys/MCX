#include "mcx/server.hpp"
#include "mcx/log.hpp"

#include <iostream>

namespace mcx {

Server::Server(Config config)
    : config_(std::move(config)) {
    if (config_.scriptRoot.empty()) {
        config_.scriptRoot = "scripts";
    }

    scriptRuntime_ = std::make_unique<DummyScriptRuntime>();
}

void Server::Start() {
    std::cout << "[MCX] Starting MCX C++ core prototype..."
              << std::endl;

    if (!config_.backendEndpoint.empty()) {
        std::cout << "[MCX] External backend endpoint: "
                  << config_.backendEndpoint << std::endl;
    } else {
        std::cout << "[MCX] No external backend configured yet."
                  << std::endl;
    }

    scriptRuntime_->LoadScripts(config_.scriptRoot);
    // In V1 this is where we will plug in the Minecraft server event source.
}

ActionList Server::HandleEvent(const Event& event) {
    PrintEvent(event);
    return scriptRuntime_->HandleEvent(event);
}

} // namespace mcx
