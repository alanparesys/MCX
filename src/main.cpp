#include "mcx/server.hpp"
#include "mcx/events.hpp"

int main() {
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

    return 0;
}
