#pragma once

#include "mcx/export.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace mcx {

struct StatValue {
    int64_t kills{0};
    int64_t deaths{0};
    int64_t blocksMined{0};
    int64_t blocksPlaced{0};
    int64_t playTimeMinutes{0};
    int64_t distanceTraveled{0};
};

class MCX_API StatsManager {
public:
    void RecordKill(const std::string& playerId);
    void RecordDeath(const std::string& playerId);
    void RecordBlockMined(const std::string& playerId);
    void RecordBlockPlaced(const std::string& playerId);
    void RecordPlayTime(const std::string& playerId, int minutes);
    void RecordDistance(const std::string& playerId, int blocks);
    
    [[nodiscard]] StatValue GetStats(const std::string& playerId) const;
    [[nodiscard]] std::vector<std::pair<std::string, int64_t>> GetTopKills(size_t count) const;
    [[nodiscard]] double GetKDRatio(const std::string& playerId) const;
    
    void SaveToFile(const std::string& path);
    void LoadFromFile(const std::string& path);

private:
    std::unordered_map<std::string, StatValue> stats_;
};

} // namespace mcx
