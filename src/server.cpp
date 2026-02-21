#include "mcx/server.hpp"

#include <iostream>

namespace mcx {

Server::Server(Config config)
    : config_(std::move(config)) {}

void Server::Start() {
    std::cout << "[MCX] Starting MCX C++ core prototype..." << std::endl;

    if (!config_.backendEndpoint.empty()) {
        std::cout << "[MCX] External backend endpoint: " << config_.backendEndpoint << std::endl;
    } else {
        std::cout << "[MCX] No external backend configured yet." << std::endl;
    }

    // In V1 this is where we will plug in the Minecraft server event source.
}

ActionList Server::HandleEvent(const Event& event) {
    // For now, just log the event type and return no actions.
    switch (event.type) {
        case EVENT_TYPE::PLAYER_JOIN:
            std::cout << "[MCX] Event: player_join" << std::endl;
            break;
        case EVENT_TYPE::PLAYER_QUIT:
            std::cout << "[MCX] Event: player_quit" << std::endl;
            break;
        case EVENT_TYPE::CHAT:
            std::cout << "[MCX] Event: chat" << std::endl;
            break;
        case EVENT_TYPE::COMMAND:
            std::cout << "[MCX] Event: command" << std::endl;
            break;
        case EVENT_TYPE::SCENE_LOADED:
            std::cout << "[MCX] Event: scene_loaded" << std::endl;
            break;
    }

    return {};
}

} // namespace mcx
