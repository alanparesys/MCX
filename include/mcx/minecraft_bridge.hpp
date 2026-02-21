#pragma once

#include "mcx/server.hpp"

namespace mcx {

class MinecraftBridge {
public:
    virtual ~MinecraftBridge() = default;

    // Start listening to the Minecraft server and forward events
    // to the MCX server instance.
    virtual void Start(Server& server) = 0;
};

class DummyMinecraftBridge : public MinecraftBridge {
public:
    void Start(Server& server) override;
};

} // namespace mcx
