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
    server.Start();

    mcx::ActionApplier applier{server.GetSceneManager()};

    mcx::log::Info("Demo run starting");

    const auto events = mcx::BuildFakeEvents();
    for (const auto& event : events) {
        auto actions = server.HandleEvent(event);
        applier.Apply(actions);
        mcx::log::Info("---");
    }

    mcx::log::Info("Demo run complete");
}

void PrintUsage() {
    std::cout << "Usage: mcx [--demo] [--version]" << std::endl;
}

void PrintVersion() {
    std::cout << "MCX version 0.0.1" << std::endl;
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
