#include "mcx/minecraft_bridge.hpp"

#include <iostream>

namespace mcx {

void DummyMinecraftBridge::Start(Server& server) {
    std::cout << "[MCX] DummyMinecraftBridge.Start() called" << std::endl;
    (void)server;
}

} // namespace mcx
