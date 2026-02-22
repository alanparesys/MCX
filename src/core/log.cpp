#include "mcx/log.hpp"

#include <iostream>

namespace mcx::log {

void Info(const std::string& message) {
    std::cout << "[MCX] " << message << std::endl;
}

void Warn(const std::string& message) {
    std::cout << "[MCX][WARN] " << message << std::endl;
}

void Error(const std::string& message) {
    std::cerr << "[MCX][ERROR] " << message << std::endl;
}

} // namespace mcx::log
