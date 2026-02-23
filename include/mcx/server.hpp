#pragma once

#include <memory>
#include <string>
#include <atomic>

#include "mcx/export.hpp"
#include "mcx/actions.hpp"
#include "mcx/events.hpp"
#include "mcx/script_runtime.hpp"

namespace mcx {

class SceneManager;
class PlayerRegistry;
class Scheduler;
class ThreadScheduler;
struct Metrics;
class PermissionRegistry;
class ModuleLoader;

struct Config {
    std::string backendEndpoint{};
    std::string scriptRoot{"scripts"};
    std::string modulesPath{"modules"};
    int maxPlayers{0};
    bool demoMode{true};
};

class MCX_API Server {
public:
    explicit Server(Config config = {});
    ~Server();

    void Run();
    void Stop();
    void Update();

    [[nodiscard]] ActionList HandleEvent(const Event& event);
    [[nodiscard]] SceneManager& GetSceneManager();
    [[nodiscard]] PlayerRegistry& GetPlayerRegistry();
    [[nodiscard]] Scheduler& GetScheduler();
    [[nodiscard]] ThreadScheduler& GetThreadScheduler();
    [[nodiscard]] Metrics& GetMetrics();
    [[nodiscard]] PermissionRegistry& GetPermissions();
    [[nodiscard]] ModuleLoader& GetModuleLoader();

private:
    void MainLoop();
    void LoadModules();

    Config config_;
    std::atomic<bool> running_{false};
    std::unique_ptr<ScriptRuntime> scriptRuntime_;
    std::unique_ptr<SceneManager> sceneManager_;
    std::unique_ptr<PlayerRegistry> playerRegistry_;
    std::unique_ptr<Scheduler> scheduler_;
    std::unique_ptr<ThreadScheduler> threadScheduler_;
    std::unique_ptr<Metrics> metrics_;
    std::unique_ptr<PermissionRegistry> permissions_;
    std::unique_ptr<ModuleLoader> moduleLoader_;
};

} // namespace mcx
