#pragma once

#include "mcx/export.hpp"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace mcx {

using CommandHandler = std::function<void(const std::vector<std::string>& args)>;

struct CommandInfo {
    std::string name;
    std::string description;
    std::string permission;
};

class MCX_API CommandRegistry {
public:
    void Register(const std::string& name, CommandInfo info, CommandHandler handler);
    void Unregister(const std::string& name);
    bool Execute(const std::string& name, const std::vector<std::string>& args);
    [[nodiscard]] std::vector<CommandInfo> GetCommands() const;

private:
    struct Entry {
        CommandInfo info;
        CommandHandler handler;
    };
    std::unordered_map<std::string, Entry> commands_;
};

} // namespace mcx
