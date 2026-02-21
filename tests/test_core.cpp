#include "mcx/server.hpp"
#include "mcx/events.hpp"

#include <iostream>

int main() {
    mcx::Config config{};
    mcx::Server server{config};

    mcx::Player player{};
    player.id = "uuid-test";
    player.name = "TestPlayer";

    mcx::ChatEvent chatEvent{};
    chatEvent.player = player;
    chatEvent.message = "!test";

    mcx::Event event{};
    event.type = mcx::EVENT_TYPE::CHAT;
    event.chat = chatEvent;

    const auto actions = server.HandleEvent(event);
    std::cout << "[MCX_TEST] actions size: "
              << actions.size() << std::endl;

    return 0;
}
