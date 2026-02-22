#pragma once

#include <string>

namespace mcx::log {

void Info(const std::string& message);
void Warn(const std::string& message);
void Error(const std::string& message);

} // namespace mcx::log
