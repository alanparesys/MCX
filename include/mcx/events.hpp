#pragma once

#include <string>
#include <optional>

namespace mcx {

struct Vec3 {
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

struct Player {
    std::string id;   // stable identifier (UUID or similar)
    std::string name; // display / username
    std::optional<Vec3> position;
};

enum class EventType {
    PlayerJoin,
    PlayerQuit,
    Chat,
    Command,
    SceneLoaded,
};

struct PlayerJoinEvent {
    EventType type{EventType::PlayerJoin};
    Player player;
};

struct PlayerQuitEvent {
    EventType type{EventType::PlayerQuit};
    Player player;
};

struct ChatEvent {
    EventType type{EventType::Chat};
    Player player;
    std::string message;
};

struct CommandEvent {
    EventType type{EventType::Command};
    std::optional<Player> player; // console commands may not have a player
    std::string command;
    std::vector<std::string> args;
};

struct SceneLoadedEvent {
    EventType type{EventType::SceneLoaded};
    std::string scene;
};

// Simple tagged variant-style container for early prototypes.
struct Event {
    EventType type{EventType::PlayerJoin};
    std::optional<PlayerJoinEvent> playerJoin;
    std::optional<PlayerQuitEvent> playerQuit;
    std::optional<ChatEvent> chat;
    std::optional<CommandEvent> command;
    std::optional<SceneLoadedEvent> sceneLoaded;
};

} // namespace mcx
