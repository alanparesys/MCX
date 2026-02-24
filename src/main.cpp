#include "mcx/server.hpp"
#include "mcx/events.hpp"
#include "mcx/action_applier.hpp"
#include "mcx/log.hpp"
#include "mcx/thread_scheduler.hpp"
#include "mcx/metrics.hpp"
#include "mcx/setup_wizard.hpp"
#include "mcx/update_checker.hpp"

#include <iostream>
#include <string>
#include <chrono>

namespace {

void RunDemo() {
    mcx::Config config{};
    config.demoMode = true;
    config.maxPlayers = 2;

    // Use hardcoded runtime for demo responses
    mcx::Server server{config};
    
    mcx::ActionApplier applier{server.GetSceneManager()};
    auto& threadScheduler = server.GetThreadScheduler();

    mcx::log::Info("Demo starting - press Ctrl+C to stop");
    mcx::log::Info("Try commands: !hello, !scene test");

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
    std::cout << "Usage: mcx [--demo] [--setup] [--version] [--update]\n";
    std::cout << "  --demo    Run demo mode\n";
    std::cout << "  --setup   Interactive server setup\n";
    std::cout << "  --version Show version\n";
    std::cout << "  --update  Check for updates\n";
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

constexpr const char* MCX_VERSION = "v0.2.0";

void PrintVersion() {
    std::cout << "MCX Server " << MCX_VERSION << "\n";
}

void CheckForUpdates() {
    std::cout << "\nChecking for updates...\n";
    
    mcx::UpdateChecker checker;
    checker.OnUpdateAvailable([](const mcx::ReleaseInfo& info) {
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════════╗\n";
        std::cout << "║  UPDATE AVAILABLE                              ║\n";
        std::cout << "╠════════════════════════════════════════════════╣\n";
        std::cout << "║  Current:  " << MCX_VERSION << "\n";
        std::cout << "║  Latest:   " << info.version << "\n";
        std::cout << "╠════════════════════════════════════════════════╣\n";
        std::cout << "║  Download: " << info.downloadUrl << "\n";
        std::cout << "╚════════════════════════════════════════════════╝\n";
        std::cout << "\nWould you like to download? (y/n): ";
        
        char response;
        std::cin >> response;
        if (response == 'y' || response == 'Y') {
            std::cout << "\nOpening browser to download page...\n";
            std::cout << "Please download and extract the new version.\n";
            #ifdef _WIN32
                system(("start "" \"" + info.downloadUrl + "\"").c_str());
            #else
                system(("xdg-open \"" + info.downloadUrl + "\"").c_str());
            #endif
        }
    });
    
    checker.CheckForUpdates(MCX_VERSION);
}

} // namespace

int main(int argc, char** argv) {
    PrintBanner();
    
    // Check for updates on every run (unless --version or specific flags)
    bool skipUpdateCheck = false;
    if (argc > 1) {
        std::string firstArg = argv[1];
        if (firstArg == "--version" || firstArg == "--update") {
            skipUpdateCheck = true;
        }
    }
    
    if (!skipUpdateCheck && argc <= 1) {
        // Interactive mode - ask about updates
        std::cout << "\nCurrent version: " << MCX_VERSION << "\n";
        std::cout << "Check for updates? (y/n): ";
        char response;
        std::cin >> response;
        if (response == 'y' || response == 'Y') {
            CheckForUpdates();
        }
    }
    
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
    
    if (arg == "--update") {
        CheckForUpdates();
        return 0;
    }

    PrintUsage();
    return 1;
}
