#include "mcx/signal_handler.hpp"

#include <csignal>

namespace mcx {

static std::atomic<bool> g_shutdownRequested{false};
static ShutdownCallback g_callback;

extern "C" void SignalHandler(int) {
    g_shutdownRequested.store(true);
    if (g_callback) {
        g_callback();
    }
}

void RegisterSignalHandler(ShutdownCallback callback) {
    g_callback = std::move(callback);
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);
}

bool ShouldShutdown() {
    return g_shutdownRequested.load();
}

} // namespace mcx
