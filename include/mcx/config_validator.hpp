#pragma once

#include "mcx/server.hpp"

#include <optional>
#include <string>

namespace mcx {

std::optional<std::string> ValidateConfig(const Config& config);

} // namespace mcx
