#include "mcx/ban_manager.hpp"
#include "mcx/log.hpp"

#include <fstream>
#include <algorithm>

namespace mcx {

void BanManager::Ban(const std::string& playerId, const std::string& reason,
                     std::chrono::hours duration) {
    BanEntry entry;
    entry.playerId = playerId;
    entry.reason = reason;
    entry.expires = std::chrono::system_clock::now() + duration;
    entry.permanent = false;
    
    bans_[playerId] = entry;
    log::Info("Player banned: " + playerId + " for " + reason);
}

void BanManager::BanPermanent(const std::string& playerId, const std::string& reason) {
    BanEntry entry;
    entry.playerId = playerId;
    entry.reason = reason;
    entry.permanent = true;
    
    bans_[playerId] = entry;
    log::Info("Player permanently banned: " + playerId);
}

void BanManager::Unban(const std::string& playerId) {
    bans_.erase(playerId);
    log::Info("Player unbanned: " + playerId);
}

bool BanManager::IsBanned(const std::string& playerId) const {
    auto it = bans_.find(playerId);
    if (it == bans_.end()) return false;
    
    if (it->second.permanent) return true;
    
    return std::chrono::system_clock::now() < it->second.expires;
}

std::vector<BanEntry> BanManager::GetActiveBans() const {
    std::vector<BanEntry> active;
    auto now = std::chrono::system_clock::now();
    
    for (const auto& [id, entry] : bans_) {
        if (entry.permanent || now < entry.expires) {
            active.push_back(entry);
        }
    }
    return active;
}

void BanManager::LoadFromFile(const std::string& path) {
    // JSON parsing omitted for brevity
    log::Info("Ban list loaded from " + path);
}

void BanManager::SaveToFile(const std::string& path) {
    // JSON serialization omitted for brevity
    log::Info("Ban list saved to " + path);
}

} // namespace mcx
