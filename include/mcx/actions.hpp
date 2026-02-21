#pragma once

#include <string>
#include <variant>
#include <vector>

#include "mcx/events.hpp"

namespace mcx {

enum class ActionKind {
    SendMessage,
    Broadcast,
    SendTitle,
    Teleport,
    SetSpawn,
    LoadScene,
};

struct SendMessageAction {
    ActionKind kind{ActionKind::SendMessage};
    std::string playerId;
    std::string text;
};

struct BroadcastAction {
    ActionKind kind{ActionKind::Broadcast};
    std::string text;
};

struct SendTitleAction {
    ActionKind kind{ActionKind::SendTitle};
    std::string playerId;
    std::string title;
    std::string subtitle;
};

struct TeleportAction {
    ActionKind kind{ActionKind::Teleport};
    std::string playerId;
    Vec3 position;
};

struct SetSpawnAction {
    ActionKind kind{ActionKind::SetSpawn};
    std::string playerId;
    Vec3 position;
};

struct LoadSceneAction {
    ActionKind kind{ActionKind::LoadScene};
    std::string scene;
};

using Action = std::variant<
    SendMessageAction,
    BroadcastAction,
    SendTitleAction,
    TeleportAction,
    SetSpawnAction,
    LoadSceneAction
>;

using ActionList = std::vector<Action>;

} // namespace mcx
