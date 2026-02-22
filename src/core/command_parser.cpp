#include "mcx/command_parser.hpp"

#include <sstream>

namespace mcx {

ParsedCommand ParseCommand(const std::string& input) {
    ParsedCommand result;
    std::istringstream stream(input);
    std::string token;
    
    if (stream >> token) {
        result.name = token;
        while (stream >> token) {
            result.args.push_back(token);
        }
    }
    return result;
}

} // namespace mcx
