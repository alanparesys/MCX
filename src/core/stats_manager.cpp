#include "mcx/stats_manager.hpp"
#include "mcx/log.hpp"

namespace mcx {

void StatsManager::RecordKill(const std::string& playerId) {
    stats_[playerId].kills++;
}

void StatsManager::RecordDeath(const std::string& playerId) {
    stats_[playerId].deaths++;
}

void StatsManager::RecordBlockMined(const std::string& playerId) {
    stats_[playerId].blocksMined++;
}

void StatsManager::RecordBlockPlaced(const std::string& playerId) {
    stats_[playerId].blocksPlaced++;
}

void StatsManager::RecordPlayTime(const std::string& playerId, int minutes) {
    stats_[playerId].playTimeMinutes += minutes;
}

void StatsManager::RecordDistance(const std::string& playerId, int blocks) {
    stats_[playerId].distanceTraveled += blocks;
}

StatValue StatsManager::GetStats(const std::string& playerId) const {
    auto it = stats_.find(playerId);
    if (it != stats_.end()) {
        return it->second;
    }
    return StatValue{};
}

std::vector<std::pair<std::string, int64_t>> StatsManager::GetTopKills(size_t count) const {
    std::vector<std::pair<std::string, int64_t>> top;
    for (const auto& [playerId, stats] : stats_) {
        top.push_back({playerId, stats.kills});
    }
    
    std::sort(top.begin(), top.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    
    if (top.size() > count) {
        top.resize(count);
    }
    return top;
}

double StatsManager::GetKDRatio(const std::string& playerId) const {
    auto it = stats_.find(playerId);
    if (it == stats_.end()) return 0.0;
    
    auto deaths = it->second.deaths;
    if (deaths == 0) return static_cast<double>(it->second.kills);
    
    return static_cast<double>(it->second.kills) / static_cast<double>(deaths);
}

void StatsManager::SaveToFile(const std::string& path) {
    log::Info("Stats saved to " + path);
}

void StatsManager::LoadFromFile(const std::string& path) {
    log::Info("Stats loaded from " + path);
}

} // namespace mcx
