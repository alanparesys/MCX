#include "mcx/events.hpp"

#include <iostream>

namespace mcx {

void PrintEvent(const Event& event) {
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
}

} // namespace mcx
