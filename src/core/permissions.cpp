#include "mcx/permissions.hpp"

namespace mcx {

void PermissionRegistry::Grant(const std::string& playerId, 
                                const std::string& permission) {
    permissions_[playerId].insert(permission);
}

void PermissionRegistry::Revoke(const std::string& playerId,
                                 const std::string& permission) {
    auto it = permissions_.find(playerId);
    if (it != permissions_.end()) {
        it->second.erase(permission);
    }
}

bool PermissionRegistry::Has(const std::string& playerId,
                              const std::string& permission) const {
    auto it = permissions_.find(playerId);
    if (it != permissions_.end()) {
        return it->second.count(permission) > 0;
    }
    return false;
}

} // namespace mcx
