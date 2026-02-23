#pragma once

#include "mcx/export.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

namespace mcx {

struct BanEntry {
    std::string playerId;
    std::string reason;
    std::chrono::system_clock::time_point expires;
    bool permanent{false};
};

class MCX_API BanManager {
public:
    void Ban(const std::string& playerId, const std::string& reason, 
             std::chrono::hours duration);
    void BanPermanent(const std::string& playerId, const std::string& reason);
    void Unban(const std::string& playerId);
    [[nodiscard]] bool IsBanned(const std::string& playerId) const;
    [[nodiscard]] std::vector<BanEntry> GetActiveBans() const;
    void LoadFromFile(const std::string& path);
    void SaveToFile(const std::string& path);

private:
    std::unordered_map<std::string, BanEntry> bans_;
};

} // namespace mcx
