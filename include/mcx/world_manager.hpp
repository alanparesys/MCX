#pragma once

#include "mcx/export.hpp"
#include <string>
#include <vector>
#include <functional>

namespace mcx {

struct WorldInfo {
    std::string name;
    std::string type; // "overworld", "nether", "end", "custom"
    bool loaded{false};
    int playerCount{0};
};

class MCX_API WorldManager {
public:
    void LoadWorld(const std::string& name);
    void UnloadWorld(const std::string& name);
    void TeleportToWorld(const std::string& playerId, const std::string& worldName);
    
    [[nodiscard]] std::vector<WorldInfo> ListWorlds() const;
    [[nodiscard]] bool WorldExists(const std::string& name) const;
    [[nodiscard]] std::string GetCurrentWorld(const std::string& playerId) const;

private:
    std::vector<WorldInfo> worlds_;
};

} // namespace mcx
