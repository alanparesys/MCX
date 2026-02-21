#include "mcx/server.hpp"
#include "mcx/events.hpp"

#include <iostream>
#include <string>

namespace {

void RunDemo() {
    mcx::Config config{};
    mcx::Server server{config};
    server.Start();

    std::cout << "[MCX] Demo run starting" << std::endl;

    const auto events = mcx::BuildFakeEvents();
    for (const auto& event : events) {
        server.HandleEvent(event);
        std::cout << "[MCX] ---" << std::endl;
    }

    std::cout << "[MCX] Demo run complete" << std::endl;
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
