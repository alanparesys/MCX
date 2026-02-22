#include "mcx/actions.hpp"
#include "mcx/log.hpp"

#include <iostream>

namespace mcx {

void ApplyActionsToLog(const ActionList& actions) {
    if (actions.empty()) {
        log::Info("No actions to apply");
        return;
    }

    for (const auto& action : actions) {
        std::visit(
            [](const auto& concreteAction) {
                using T = std::decay_t<decltype(concreteAction)>;

                if constexpr (std::is_same_v<T, SendMessageAction>) {
                    log::Info("Action: sendMessage -> playerId="
                              + concreteAction.playerId + ", text="
                              + concreteAction.text);
                } else if constexpr (std::is_same_v<T, BroadcastAction>) {
                    log::Info("Action: broadcast -> text="
                              + concreteAction.text);
                } else if constexpr (std::is_same_v<T, SendTitleAction>) {
                    log::Info("Action: sendTitle -> playerId="
                              + concreteAction.playerId + ", title="
                              + concreteAction.title);
                } else if constexpr (std::is_same_v<T, TeleportAction>) {
                    log::Info("Action: teleport -> playerId="
                              + concreteAction.playerId + ", pos=("
                              + std::to_string(concreteAction.position.x)
                              + ", "
                              + std::to_string(concreteAction.position.y)
                              + ", "
                              + std::to_string(concreteAction.position.z)
                              + ")");
                } else if constexpr (std::is_same_v<T, SetSpawnAction>) {
                    log::Info("Action: setSpawn -> playerId="
                              + concreteAction.playerId + ", pos=("
                              + std::to_string(concreteAction.position.x)
                              + ", "
                              + std::to_string(concreteAction.position.y)
                              + ", "
                              + std::to_string(concreteAction.position.z)
                              + ")");
                } else if constexpr (std::is_same_v<T, LoadSceneAction>) {
                    log::Info("Action: loadScene -> scene="
                              + concreteAction.scene);
                }
            },
            action);
    }
}

} // namespace mcx
