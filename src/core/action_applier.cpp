#include "mcx/action_applier.hpp"
#include "mcx/scene_manager.hpp"
#include "mcx/log.hpp"

#include <variant>

namespace mcx {

ActionApplier::ActionApplier(SceneManager& sceneManager)
    : sceneManager_(sceneManager) {
}

void ActionApplier::Apply(const ActionList& actions) {
    if (actions.empty()) {
        log::Info("No actions to apply");
        return;
    }

    for (const auto& action : actions) {
        std::visit(
            [this](const auto& concreteAction) {
                using T = std::decay_t<decltype(concreteAction)>;

                if constexpr (std::is_same_v<T,
                              SendMessageAction>) {
                    log::Info("Action: sendMessage -> playerId="
                              + std::string(concreteAction.playerId)
                              + ", text="
                              + std::string(concreteAction.text));
                } else if constexpr (std::is_same_v<T,
                                     BroadcastAction>) {
                    log::Info("Action: broadcast -> text="
                              + std::string(concreteAction.text));
                } else if constexpr (std::is_same_v<T,
                                     SendTitleAction>) {
                    log::Info("Action: sendTitle -> playerId="
                              + std::string(concreteAction.playerId)
                              + ", title="
                              + std::string(concreteAction.title));
                } else if constexpr (std::is_same_v<T,
                                     TeleportAction>) {
                    log::Info("Action: teleport -> playerId="
                              + std::string(concreteAction.playerId)
                              + ", pos=("
                              + std::to_string(
                                    concreteAction.position.x)
                              + ", "
                              + std::to_string(
                                    concreteAction.position.y)
                              + ", "
                              + std::to_string(
                                    concreteAction.position.z)
                              + ")");
                } else if constexpr (std::is_same_v<T,
                                     SetSpawnAction>) {
                    log::Info("Action: setSpawn -> playerId="
                              + std::string(concreteAction.playerId)
                              + ", pos=("
                              + std::to_string(
                                    concreteAction.position.x)
                              + ", "
                              + std::to_string(
                                    concreteAction.position.y)
                              + ", "
                              + std::to_string(
                                    concreteAction.position.z)
                              + ")");
                } else if constexpr (std::is_same_v<T,
                                     LoadSceneAction>) {
                    sceneManager_.LoadScene(
                        concreteAction.scene);
                }
            },
            action);
    }
}

} // namespace mcx
