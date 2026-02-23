#include "mcx/server.hpp"
#include "mcx/events.hpp"
#include "mcx/action_applier.hpp"
#include "mcx/log.hpp"
#include "mcx/thread_scheduler.hpp"
#include "mcx/metrics.hpp"
#include "mcx/setup_wizard.hpp"

#include <iostream>
#include <string>
#include <chrono>

namespace {

void RunDemo() {
    mcx::Config config{};
    config.demoMode = true;
    config.maxPlayers = 2;

    mcx::Server server{config};
    
    mcx::ActionApplier applier{server.GetSceneManager()};
    auto& threadScheduler = server.GetThreadScheduler();

    mcx::log::Info("Demo starting - press Ctrl+C to stop");

    threadScheduler.Schedule(std::chrono::seconds(1), []() {
        mcx::log::Info("Scheduled task executed");
    });

    const auto events = mcx::BuildFakeEvents();
    for (const auto& event : events) {
        auto actions = server.HandleEvent(event);
        applier.Apply(actions);
        server.Update();
    }

    mcx::log::Info("Demo complete");
    mcx::log::Info("Events: " + std::to_string(server.GetMetrics().eventsProcessed.load()));
    mcx::log::Info("Actions: " + std::to_string(server.GetMetrics().actionsExecuted.load()));
}

void RunSetup() {
    mcx::SetupWizard wizard;
    if (wizard.Run()) {
        std::cout << "\nStarting server...\n";
        
        auto cfg = wizard.GetConfig();
        mcx::Config config{};
        config.maxPlayers = cfg.maxPlayers;
        config.backendEndpoint = cfg.bindIp + ":" + std::to_string(cfg.port);
        
        mcx::Server server{config};
        server.Run();
    }
}

void PrintUsage() {
    std::cout << "Usage: mcx [--demo] [--setup] [--version]\n";
    std::cout << "  --demo    Run demo mode\n";
    std::cout << "  --setup   Interactive server setup\n";
    std::cout << "  --version Show version\n";
}

void PrintBanner() {
    std::cout << R"(
    _..._
  .'     '.
 |  __ __  |
 | |  |  | |
 | |  |  | |  ___  ___ ___ ___
 | |  |  | | / __|/ __/ __/ __|
 | |  |  | || (__| (__\__ \__ \
 | |  |  | | \___|\___|___/___/
 |.|  |  |.|
 '..'  '..'
  ||    ||
  ''    ''
)" << std::endl;
}

void PrintVersion() {
    std::cout << "MCX Server v0.2.0\n";
}

} // namespace

int main(int argc, char** argv) {
    PrintBanner();
    
    if (argc <= 1) {
        PrintUsage();
        return 0;
    }

    const std::string arg = argv[1];

    if (arg == "--demo") {
        RunDemo();
        return 0;
    }

    if (arg == "--setup") {
        RunSetup();
        return 0;
    }

    if (arg == "--version") {
        PrintVersion();
        return 0;
    }

    PrintUsage();
    return 1;
}
