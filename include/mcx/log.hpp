#pragma once

#include <string_view>

namespace mcx::log {

void Info(std::string_view message);
void Warn(std::string_view message);
void Error(std::string_view message);

} // namespace mcx::log
