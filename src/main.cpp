#include "mcx/server.hpp"
#include "mcx/events.hpp"
#include "mcx/action_applier.hpp"
#include "mcx/log.hpp"
#include "mcx/signal_handler.hpp"
#include "mcx/scheduler.hpp"

#include <iostream>
#include <string>

namespace {

void RunDemo() {
    mcx::RegisterSignalHandler([]() {
        mcx::log::Info("Shutdown requested...");
    });

    mcx::Config config{};
    config.demoMode = true;
    config.maxPlayers = 2;

    mcx::Server server{config};
    server.Start();

    mcx::ActionApplier applier{server.GetSceneManager()};
    auto& scheduler = server.GetScheduler();

    mcx::log::Info("Demo run starting");

    scheduler.Schedule(std::chrono::seconds(1), []() {
        mcx::log::Info("Scheduled: Demo timer fired");
    });

    const auto events = mcx::BuildFakeEvents();
    for (const auto& event : events) {
        if (mcx::ShouldShutdown()) break;
        auto actions = server.HandleEvent(event);
        applier.Apply(actions);
        scheduler.Update();
        mcx::log::Info("---");
    }

    scheduler.Clear();

    mcx::log::Info("Demo run complete");
    mcx::log::Info("Events processed: " + 
        std::to_string(server.GetMetrics().eventsProcessed.load()));
}

void PrintUsage() {
    std::cout << "Usage: mcx [--demo] [--version]" << std::endl;
}

void PrintVersion() {
    std::cout << "MCX version 0.1.0" << std::endl;
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

    PrintUsage();
    return 1;
}
