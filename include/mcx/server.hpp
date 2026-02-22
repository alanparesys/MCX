#pragma once

#include <memory>
#include <string>

#include "mcx/actions.hpp"
#include "mcx/events.hpp"
#include "mcx/script_runtime.hpp"

namespace mcx {

struct Config {
    std::string backendEndpoint{};
    std::string scriptRoot{"scripts"};
    int maxPlayers{0};
    bool demoMode{true};
};

class SceneManager;
class PlayerRegistry;

class Server {
public:
    explicit Server(Config config = {});

    void Start();

    // Handle an incoming event from the Minecraft server and return actions
    // MCX would like the server to perform.
    [[nodiscard]] ActionList HandleEvent(const Event& event);

    // Returns the scene manager for applying actions.
    [[nodiscard]] SceneManager& GetSceneManager();

    // Returns the player registry for tracking connected players.
    [[nodiscard]] PlayerRegistry& GetPlayerRegistry();

private:
    Config config_;
    std::unique_ptr<ScriptRuntime> scriptRuntime_;
    std::unique_ptr<SceneManager> sceneManager_;
    std::unique_ptr<PlayerRegistry> playerRegistry_;
};

} // namespace mcx