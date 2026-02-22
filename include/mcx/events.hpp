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
    Player player;
};

struct PlayerQuitEvent {
    Player player;
};

struct ChatEvent {
    Player player;
    std::string message;
};

struct CommandEvent {
    std::optional<Player> player;
    std::string command;
    std::vector<std::string> args;
};

struct SceneLoadedEvent {
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
