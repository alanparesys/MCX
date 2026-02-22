#pragma once

#include <string>
#include <vector>

namespace mcx {

struct ParsedCommand {
    std::string name;
    std::vector<std::string> args;
};

ParsedCommand ParseCommand(const std::string& input);

} // namespace mcx
