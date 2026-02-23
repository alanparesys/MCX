#pragma once

#include "mcx/export.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace mcx {

struct KitItem {
    std::string itemId;
    int count{1};
    std::string enchantments;
};

struct Kit {
    std::string name;
    std::vector<KitItem> items;
    int cooldownSeconds{0};
    std::string permission;
};

class MCX_API KitManager {
public:
    void DefineKit(const std::string& name, const Kit& kit);
    void DeleteKit(const std::string& name);
    
    bool GiveKit(const std::string& playerId, const std::string& kitName);
    [[nodiscard]] bool CanReceiveKit(const std::string& playerId, const std::string& kitName) const;
    
    [[nodiscard]] std::vector<std::string> ListKits() const;
    [[nodiscard]] Kit GetKit(const std::string& name) const;
    
    void LoadKitsFromFile(const std::string& path);
    void SaveKitsToFile(const std::string& path);

private:
    std::unordered_map<std::string, Kit> kits_;
    std::unordered_map<std::string, std::unordered_map<std::string, int64_t>> lastReceived_;
    
    int64_t GetCurrentTimestamp() const;
};

} // namespace mcx
