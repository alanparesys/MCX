#pragma once

#include <string>
#include <variant>
#include <vector>

#include "mcx/events.hpp"

namespace mcx {

enum class ACTION_KIND {
    SEND_MESSAGE,
    BROADCAST,
    SEND_TITLE,
    TELEPORT,
    SET_SPAWN,
    LOAD_SCENE,
};

struct SendMessageAction {
    ACTION_KIND kind{ACTION_KIND::SEND_MESSAGE};
    std::string playerId;
    std::string text;
};

struct BroadcastAction {
    ACTION_KIND kind{ACTION_KIND::BROADCAST};
    std::string text;
};

struct SendTitleAction {
    ACTION_KIND kind{ACTION_KIND::SEND_TITLE};
    std::string playerId;
    std::string title;
    std::string subtitle;
};

struct TeleportAction {
    ACTION_KIND kind{ACTION_KIND::TELEPORT};
    std::string playerId;
    Vec3 position;
};

struct SetSpawnAction {
    ACTION_KIND kind{ACTION_KIND::SET_SPAWN};
    std::string playerId;
    Vec3 position;
};

struct LoadSceneAction {
    ACTION_KIND kind{ACTION_KIND::LOAD_SCENE};
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

// For early prototypes, apply actions by logging them.
void ApplyActionsToLog(const ActionList& actions);

} // namespace mcx
