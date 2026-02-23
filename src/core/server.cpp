#include "mcx/server.hpp"
#include "mcx/log.hpp"
#include "mcx/scene_manager.hpp"
#include "mcx/player_registry.hpp"
#include "mcx/scheduler.hpp"
#include "mcx/thread_scheduler.hpp"
#include "mcx/metrics.hpp"
#include "mcx/permissions.hpp"
#include "mcx/module_loader.hpp"
#include "mcx/lua_runtime.hpp"
#include "mcx/signal_handler.hpp"

#include <filesystem>
#include <thread>

namespace mcx {

Server::Server(Config config)
    : config_(std::move(config)) {
    scriptRuntime_ = CreateLuaRuntime();
    sceneManager_ = std::make_unique<SceneManager>();
    playerRegistry_ = std::make_unique<PlayerRegistry>();
    scheduler_ = std::make_unique<Scheduler>();
    threadScheduler_ = std::make_unique<ThreadScheduler>();
    metrics_ = std::make_unique<Metrics>();
    permissions_ = std::make_unique<PermissionRegistry>();
    moduleLoader_ = std::make_unique<ModuleLoader>();
}

Server::~Server() = default;

void Server::Run() {
    running_ = true;
    
    RegisterSignalHandler([this]() {
        log::Info("Shutdown signal received");
        Stop();
    });

    log::Info("Starting MCX server v0.2.0");
    
    scriptRuntime_->LoadScripts(config_.scriptRoot);
    LoadModules();
    
    try {
        MainLoop();
    } catch (const std::exception& e) {
        log::Error("Fatal server error: " + std::string(e.what()));
        Stop();
        throw;
    }
}

void Server::Stop() {
    running_ = false;
}

void Server::MainLoop() {
    while (running_ && !ShouldShutdown()) {
        try {
            Update();
        } catch (const std::exception& e) {
            log::Error("Frame error: " + std::string(e.what()));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Server::Update() {
    scheduler_->Update();
}

void Server::LoadModules() {
    if (!std::filesystem::exists(config_.modulesPath)) {
        log::Warn("Modules path not found: " + config_.modulesPath);
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(config_.modulesPath)) {
#ifdef _WIN32
        if (entry.path().extension() == ".dll") {
#else
        if (entry.path().extension() == ".so") {
#endif
            moduleLoader_->LoadModule(entry.path());
        }
    }
}

ActionList Server::HandleEvent(const Event& event) {
    metrics_->eventsProcessed++;
    
    if (event.type == EVENT_TYPE::PLAYER_JOIN && event.playerJoin.has_value()) {
        const auto& joinEvent = event.playerJoin.value();
        Player player{};
        player.id = joinEvent.player.id;
        player.name = joinEvent.player.name;
        player.scene = sceneManager_->GetCurrentScene();
        playerRegistry_->AddPlayer(player);
        metrics_->playersConnected++;
    } else if (event.type == EVENT_TYPE::PLAYER_QUIT && event.playerQuit.has_value()) {
        const auto& quitEvent = event.playerQuit.value();
        playerRegistry_->RemovePlayer(quitEvent.player.id);
        metrics_->playersConnected--;
    }

    ActionList actions = scriptRuntime_->HandleEvent(event);
    moduleLoader_->DispatchEvent(event, actions);
    
    metrics_->actionsExecuted += actions.size();
    return actions;
}

SceneManager& Server::GetSceneManager() {
    return *sceneManager_;
}

PlayerRegistry& Server::GetPlayerRegistry() {
    return *playerRegistry_;
}

Scheduler& Server::GetScheduler() {
    return *scheduler_;
}

ThreadScheduler& Server::GetThreadScheduler() {
    return *threadScheduler_;
}

Metrics& Server::GetMetrics() {
    return *metrics_;
}

PermissionRegistry& Server::GetPermissions() {
    return *permissions_;
}

ModuleLoader& Server::GetModuleLoader() {
    return *moduleLoader_;
}

} // namespace mcx
