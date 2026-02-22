#include "mcx/player_registry.hpp"
#include "mcx/log.hpp"

namespace mcx {

void PlayerRegistry::AddPlayer(const Player& player) {
    players_[player.id] = player;
    log::Info("Player joined: " + player.name +
              " (" + player.id + ")");
}

void PlayerRegistry::RemovePlayer(const std::string& playerId) {
    auto it = players_.find(playerId);
    if (it != players_.end()) {
        log::Info("Player left: " + it->second.name +
                  " (" + playerId + ")");
        players_.erase(it);
    } else {
        log::Warn("Attempted to remove unknown player: " +
                  playerId);
    }
}

std::optional<Player> PlayerRegistry::GetPlayer(
    const std::string& playerId) const {
    auto it = players_.find(playerId);
    if (it != players_.end()) {
        return it->second;
    }
    return std::nullopt;
}

size_t PlayerRegistry::GetOnlineCount() const {
    return players_.size();
}

void PlayerRegistry::UpdatePlayerScene(const std::string& playerId,
                                       const std::string& scene) {
    auto it = players_.find(playerId);
    if (it != players_.end()) {
        it->second.scene = scene;
        log::Info("Player " + it->second.name +
                  " moved to scene: " + scene);
    } else {
        log::Warn("Cannot update scene for unknown player: " +
                  playerId);
    }
}

} // namespace mcx
