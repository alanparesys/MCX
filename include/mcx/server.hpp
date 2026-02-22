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
class Scheduler;
struct Metrics;
class PermissionRegistry;

class Server {
public:
    explicit Server(Config config = {});

    void Start();

    [[nodiscard]] ActionList HandleEvent(const Event& event);

    [[nodiscard]] SceneManager& GetSceneManager();
    [[nodiscard]] PlayerRegistry& GetPlayerRegistry();
    [[nodiscard]] Scheduler& GetScheduler();
    [[nodiscard]] Metrics& GetMetrics();
    [[nodiscard]] PermissionRegistry& GetPermissions();

private:
    Config config_;
    std::unique_ptr<ScriptRuntime> scriptRuntime_;
    std::unique_ptr<SceneManager> sceneManager_;
    std::unique_ptr<PlayerRegistry> playerRegistry_;
    std::unique_ptr<Scheduler> scheduler_;
    std::unique_ptr<Metrics> metrics_;
    std::unique_ptr<PermissionRegistry> permissions_;
};

} // namespace mcx
