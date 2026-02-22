#pragma once

#include "mcx/server.hpp"

#include <optional>
#include <string>

namespace mcx {

// Attempt to load configuration from a JSON file.
// Returns std::nullopt if the file cannot be read or parsed.
std::optional<Config> LoadConfigFromFile(const std::string& path);

} // namespace mcx
