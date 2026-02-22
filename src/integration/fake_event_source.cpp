#include "mcx/events.hpp"

#include <vector>

namespace mcx {

std::vector<Event> BuildFakeEvents() {
    std::vector<Event> events{};

    Player player{};
    player.id = "uuid-1234";
    player.name = "TestPlayer";

    PlayerJoinEvent joinEvent{};
    joinEvent.player = player;

    Event join{};
    join.type = EVENT_TYPE::PLAYER_JOIN;
    join.playerJoin = joinEvent;
    events.push_back(join);

    ChatEvent chatHello{};
    chatHello.player = player;
    chatHello.message = "!hello";

    Event chatHelloEvent{};
    chatHelloEvent.type = EVENT_TYPE::CHAT;
    chatHelloEvent.chat = chatHello;
    events.push_back(chatHelloEvent);

    ChatEvent chatScene{};
    chatScene.player = player;
    chatScene.message = "!scene test";

    Event chatSceneEvent{};
    chatSceneEvent.type = EVENT_TYPE::CHAT;
    chatSceneEvent.chat = chatScene;
    events.push_back(chatSceneEvent);

    CommandEvent commandEvent{};
    commandEvent.player = player;
    commandEvent.command = "mcx:test";
    commandEvent.args = {"arg1", "arg2"};

    Event command{};
    command.type = EVENT_TYPE::COMMAND;
    command.command = commandEvent;
    events.push_back(command);

    return events;
}

} // namespace mcx
