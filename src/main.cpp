#include "mcx/server.hpp"
#include "mcx/events.hpp"

int main() {
    mcx::Config config{};
    mcx::Server server{config};
    server.Start();

    // Demo: feed a few fake events into the core so we can see basic
    // behavior without a real Minecraft server yet.
    const auto events = mcx::BuildFakeEvents();
    for (const auto& event : events) {
        server.HandleEvent(event);
    }

    return 0;
}
