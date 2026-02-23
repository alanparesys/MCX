#include "mcx/kit_manager.hpp"
#include "mcx/log.hpp"

#include <chrono>

namespace mcx {

void KitManager::DefineKit(const std::string& name, const Kit& kit) {
    kits_[name] = kit;
    log::Info("Defined kit: " + name);
}

void KitManager::DeleteKit(const std::string& name) {
    kits_.erase(name);
    log::Info("Deleted kit: " + name);
}

bool KitManager::GiveKit(const std::string& playerId, const std::string& kitName) {
    if (!CanReceiveKit(playerId, kitName)) {
        return false;
    }
    
    auto it = kits_.find(kitName);
    if (it == kits_.end()) {
        return false;
    }
    
    lastReceived_[playerId][kitName] = GetCurrentTimestamp();
    log::Info("Gave kit " + kitName + " to player " + playerId);
    return true;
}

bool KitManager::CanReceiveKit(const std::string& playerId, const std::string& kitName) const {
    auto kitIt = kits_.find(kitName);
    if (kitIt == kits_.end()) {
        return false;
    }
    
    auto playerIt = lastReceived_.find(playerId);
    if (playerIt == lastReceived_.end()) {
        return true;
    }
    
    auto kitTimeIt = playerIt->second.find(kitName);
    if (kitTimeIt == playerIt->second.end()) {
        return true;
    }
    
    int64_t elapsed = GetCurrentTimestamp() - kitTimeIt->second;
    return elapsed >= kitIt->second.cooldownSeconds;
}

std::vector<std::string> KitManager::ListKits() const {
    std::vector<std::string> names;
    for (const auto& [name, kit] : kits_) {
        names.push_back(name);
    }
    return names;
}

Kit KitManager::GetKit(const std::string& name) const {
    auto it = kits_.find(name);
    if (it != kits_.end()) {
        return it->second;
    }
    return Kit{};
}

void KitManager::LoadKitsFromFile(const std::string& path) {
    log::Info("Loading kits from " + path);
}

void KitManager::SaveKitsToFile(const std::string& path) {
    log::Info("Saving kits to " + path);
}

int64_t KitManager::GetCurrentTimestamp() const {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

} // namespace mcx
