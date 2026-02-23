#include "mcx/command_registry.hpp"
#include "mcx/log.hpp"

namespace mcx {

void CommandRegistry::Register(const std::string& name, CommandInfo info, CommandHandler handler) {
    commands_[name] = {std::move(info), std::move(handler)};
}

void CommandRegistry::Unregister(const std::string& name) {
    commands_.erase(name);
}

bool CommandRegistry::Execute(const std::string& name, const std::vector<std::string>& args) {
    auto it = commands_.find(name);
    if (it == commands_.end()) {
        return false;
    }
    
    try {
        it->second.handler(args);
        return true;
    } catch (const std::exception& e) {
        log::Error("Command " + name + " failed: " + e.what());
        return false;
    }
}

std::vector<CommandInfo> CommandRegistry::GetCommands() const {
    std::vector<CommandInfo> result;
    for (const auto& [name, entry] : commands_) {
        result.push_back(entry.info);
    }
    return result;
}

} // namespace mcx
