#include "mcx/world_manager.hpp"
#include "mcx/log.hpp"

namespace mcx {

void WorldManager::LoadWorld(const std::string& name) {
    log::Info("Loading world: " + name);
    // Implementation would interface with Minecraft server
}

void WorldManager::UnloadWorld(const std::string& name) {
    log::Info("Unloading world: " + name);
}

void WorldManager::TeleportToWorld(const std::string& playerId, const std::string& worldName) {
    log::Info("Teleporting player " + playerId + " to world " + worldName);
}

std::vector<WorldInfo> WorldManager::ListWorlds() const {
    return worlds_;
}

bool WorldManager::WorldExists(const std::string& name) const {
    for (const auto& world : worlds_) {
        if (world.name == name) return true;
    }
    return false;
}

std::string WorldManager::GetCurrentWorld(const std::string& playerId) const {
    return "world"; // Default world
}

} // namespace mcx
