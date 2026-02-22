#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace mcx {

class PermissionRegistry {
public:
    void Grant(const std::string& playerId, const std::string& permission);
    void Revoke(const std::string& playerId, const std::string& permission);
    bool Has(const std::string& playerId, const std::string& permission) const;

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> permissions_;
};

} // namespace mcx
