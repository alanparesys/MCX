#include "mcx/log.hpp"

#include <iostream>

namespace mcx::log {

void Info(std::string_view message) {
    std::cout << "[MCX] " << message << '\n';
}

void Warn(std::string_view message) {
    std::cout << "[MCX][WARN] " << message << '\n';
}

void Error(std::string_view message) {
    std::cerr << "[MCX][ERROR] " << message << '\n';
}

} // namespace mcx::log
