#include "mcx/server.hpp"
#include "mcx/events.hpp"
#include "mcx/action_applier.hpp"
#include "mcx/log.hpp"

#include <iostream>
#include <string>

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

void PrintUsage() {
    std::cout << "Usage: mcx [--demo] [--version]" << std::endl;
}

void PrintVersion() {
    std::cout << "MCX Server v0.2.0" << std::endl;
}

} // namespace

int main(int argc, char** argv) {
    if (argc <= 1) {
        PrintUsage();
        return 0;
    }

    const std::string arg = argv[1];

    if (arg == "--demo") {
        RunDemo();
        return 0;
    }

    if (arg == "--version") {
        PrintVersion();
        return 0;
    }

    if (arg == "--server") {
        mcx::Server server;
        server.Run();
        return 0;
    }

    PrintUsage();
    return 1;
}
