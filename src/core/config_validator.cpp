#include "mcx/config_validator.hpp"

namespace mcx {

std::optional<std::string> ValidateConfig(const Config& config) {
    if (config.maxPlayers < 0) {
        return "maxPlayers cannot be negative";
    }
    if (config.scriptRoot.empty()) {
        return "scriptRoot cannot be empty";
    }
    return std::nullopt;
}

} // namespace mcx
