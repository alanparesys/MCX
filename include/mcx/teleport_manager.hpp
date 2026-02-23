#pragma once

#include "mcx/export.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>

namespace mcx {

struct TeleportRequest {
    std::string requesterId;
    std::string targetId;
    std::chrono::steady_clock::time_point timestamp;
};

class MCX_API TeleportManager {
public:
    void RequestTeleport(const std::string& requesterId, const std::string& targetId);
    void AcceptTeleport(const std::string& targetId);
    void DenyTeleport(const std::string& targetId);
    void CancelTeleport(const std::string& requesterId);
    
    void TeleportToPlayer(const std::string& playerId, const std::string& targetId);
    void TeleportToCoords(const std::string& playerId, double x, double y, double z);
    void TeleportToSpawn(const std::string& playerId);
    
    void SetSpawnPoint(const std::string& worldName, double x, double y, double z);
    
private:
    std::unordered_map<std::string, TeleportRequest> pendingRequests_;
    std::unordered_map<std::string, std::tuple<double, double, double>> spawnPoints_;
    
    void ClearExpiredRequests();
};

} // namespace mcx
