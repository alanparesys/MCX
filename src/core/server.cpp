#include "mcx/server.hpp"
#include "mcx/log.hpp"
#include "mcx/scene_manager.hpp"

#include <iostream>

namespace mcx {

Server::Server(Config config)
    : config_(std::move(config)) {
    if (config_.scriptRoot.empty()) {
        config_.scriptRoot = "scripts";
    }

    scriptRuntime_ = std::make_unique<DummyScriptRuntime>();
    sceneManager_ = std::make_unique<SceneManager>();
}

void Server::Start() {
    log::Info("Starting MCX C++ core prototype...");

    if (!config_.backendEndpoint.empty()) {
        log::Info("External backend endpoint: "
                  + config_.backendEndpoint);
    } else {
        log::Info("No external backend configured yet.");
    }

    log::Info("Config: demoMode="
              + std::string(config_.demoMode ? "true" : "false")
              + ", maxPlayers="
              + std::to_string(config_.maxPlayers));

    scriptRuntime_->LoadScripts(config_.scriptRoot);

    log::Info("Server started. Scene: "
              + sceneManager_->GetCurrentScene());
}

ActionList Server::HandleEvent(const Event& event) {
    PrintEvent(event);
    return scriptRuntime_->HandleEvent(event);
}

SceneManager& Server::GetSceneManager() {
    return *sceneManager_;
}

} // namespace mcx
