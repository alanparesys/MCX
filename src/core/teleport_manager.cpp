#include "mcx/teleport_manager.hpp"
#include "mcx/log.hpp"

namespace mcx {

void TeleportManager::RequestTeleport(const std::string& requesterId, const std::string& targetId) {
    TeleportRequest req{requesterId, targetId, std::chrono::steady_clock::now()};
    pendingRequests_[targetId] = req;
    log::Info("Teleport request from " + requesterId + " to " + targetId);
}

void TeleportManager::AcceptTeleport(const std::string& targetId) {
    auto it = pendingRequests_.find(targetId);
    if (it != pendingRequests_.end()) {
        log::Info("Teleport accepted by " + targetId);
        pendingRequests_.erase(it);
    }
}

void TeleportManager::DenyTeleport(const std::string& targetId) {
    auto it = pendingRequests_.find(targetId);
    if (it != pendingRequests_.end()) {
        log::Info("Teleport denied by " + targetId);
        pendingRequests_.erase(it);
    }
}

void TeleportManager::CancelTeleport(const std::string& requesterId) {
    for (auto it = pendingRequests_.begin(); it != pendingRequests_.end(); ++it) {
        if (it->second.requesterId == requesterId) {
            pendingRequests_.erase(it);
            log::Info("Teleport cancelled by " + requesterId);
            return;
        }
    }
}

void TeleportManager::TeleportToPlayer(const std::string& playerId, const std::string& targetId) {
    log::Info("Teleporting " + playerId + " to player " + targetId);
}

void TeleportManager::TeleportToCoords(const std::string& playerId, double x, double y, double z) {
    log::Info("Teleporting " + playerId + " to coordinates (" + 
              std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
}

void TeleportManager::TeleportToSpawn(const std::string& playerId) {
    log::Info("Teleporting " + playerId + " to spawn");
}

void TeleportManager::SetSpawnPoint(const std::string& worldName, double x, double y, double z) {
    spawnPoints_[worldName] = std::make_tuple(x, y, z);
    log::Info("Spawn point set for world " + worldName);
}

void TeleportManager::ClearExpiredRequests() {
    auto now = std::chrono::steady_clock::now();
    for (auto it = pendingRequests_.begin(); it != pendingRequests_.end();) {
        auto age = std::chrono::duration_cast<std::chrono::seconds>(now - it->second.timestamp).count();
        if (age > 60) {
            it = pendingRequests_.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace mcx
