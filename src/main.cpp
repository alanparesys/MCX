#include "mcx/server.hpp"

int main() {
    mcx::Config config{};
    mcx::Server server{config};
    server.Start();
    return 0;
}
