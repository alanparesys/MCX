#pragma once

#include <atomic>

namespace mcx {

struct Metrics {
    std::atomic<size_t> eventsProcessed{0};
    std::atomic<size_t> actionsExecuted{0};
    std::atomic<size_t> playersConnected{0};
    std::atomic<size_t> scenesSwitched{0};
};

} // namespace mcx
