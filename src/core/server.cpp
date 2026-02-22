#include "mcx/server.hpp"
#include "mcx/log.hpp"
#include "mcx/scene_manager.hpp"
#include "mcx/player_registry.hpp"

#include <iostream>

namespace mcx {

Server::Server(Config config)
    : config_(std::move(config)) {
    if (config_.scriptRoot.empty()) {
        config_.scriptRoot = "scripts";
    }

    scriptRuntime_ = std::make_unique<DummyScriptRuntime>();
    sceneManager_ = std::make_unique<SceneManager>();
    playerRegistry_ = std::make_unique<PlayerRegistry>();
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
              + sceneManager_->GetCurrentScene() +
              ", Players: " +
              std::to_string(playerRegistry_->GetOnlineCount()));
}

ActionList Server::HandleEvent(const Event& event) {
    PrintEvent(event);

    // Track player join/quit events
    if (event.type == EVENT_TYPE::PLAYER_JOIN &&
        event.playerJoin.has_value()) {
        const auto& joinEvent = event.playerJoin.value();
        Player player{};
        player.id = joinEvent.player.id;
        player.name = joinEvent.player.name;
        player.scene = sceneManager_->GetCurrentScene();
        playerRegistry_->AddPlayer(player);
    } else if (event.type == EVENT_TYPE::PLAYER_QUIT &&
               event.playerQuit.has_value()) {
        const auto& quitEvent = event.playerQuit.value();
        playerRegistry_->RemovePlayer(quitEvent.player.id);
    }

    return scriptRuntime_->HandleEvent(event);
}

SceneManager& Server::GetSceneManager() {
    return *sceneManager_;
}

PlayerRegistry& Server::GetPlayerRegistry() {
    return *playerRegistry_;
}

} // namespace mcx
