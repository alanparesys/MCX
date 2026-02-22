#pragma once

#include <optional>
#include <string>
#include <vector>

namespace mcx {

struct Vec3 {
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

struct Player {
    std::string id;
    std::string name;
    std::optional<Vec3> position;
    std::string scene;
};

enum class EVENT_TYPE {
    PLAYER_JOIN,
    PLAYER_QUIT,
    CHAT,
    COMMAND,
    SCENE_LOADED,
};

struct PlayerJoinEvent {
    EVENT_TYPE type{EVENT_TYPE::PLAYER_JOIN};
    Player player;
};

struct PlayerQuitEvent {
    EVENT_TYPE type{EVENT_TYPE::PLAYER_QUIT};
    Player player;
};

struct ChatEvent {
    EVENT_TYPE type{EVENT_TYPE::CHAT};
    Player player;
    std::string message;
};

struct CommandEvent {
    EVENT_TYPE type{EVENT_TYPE::COMMAND};
    std::optional<Player> player; // console commands may not have a player
    std::string command;
    std::vector<std::string> args;
};

struct SceneLoadedEvent {
    EVENT_TYPE type{EVENT_TYPE::SCENE_LOADED};
    std::string scene;
};

// Simple tagged variant-style container for early prototypes.
struct Event {
    EVENT_TYPE type{EVENT_TYPE::PLAYER_JOIN};
    std::optional<PlayerJoinEvent> playerJoin;
    std::optional<PlayerQuitEvent> playerQuit;
    std::optional<ChatEvent> chat;
    std::optional<CommandEvent> command;
    std::optional<SceneLoadedEvent> sceneLoaded;
};

// Build a small list of fake events for local testing.
std::vector<Event> BuildFakeEvents();

// Print a short log line for an event (for demo runs).
void PrintEvent(const Event& event);

} // namespace mcx
