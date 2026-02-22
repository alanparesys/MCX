#pragma once

#include <memory>
#include <string>

#include "mcx/actions.hpp"
#include "mcx/events.hpp"
#include "mcx/script_runtime.hpp"

namespace mcx {

struct Config {
    std::string backendEndpoint; // optional external backend; may be empty
    std::string scriptRoot;      // where scripts are loaded from
    int maxPlayers{0};           // optional hint for future use
    bool demoMode{true};         // whether MCX is running in demo mode
};

class Server {
public:
    explicit Server(Config config = {});

    void Start();

    // Handle an incoming event from the Minecraft server and return actions
    // MCX would like the server to perform.
    [[nodiscard]] ActionList HandleEvent(const Event& event);

private:
    Config config_;
    std::unique_ptr<ScriptRuntime> scriptRuntime_;
};

} // namespace mcx
