#pragma once

#include "mcx/export.hpp"
#include "mcx/events.hpp"
#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>

namespace mcx {

using EventHandler = std::function<void(const Event&)>;

class MCX_API EventBus {
public:
    void Subscribe(EVENT_TYPE type, EventHandler handler);
    void Publish(const Event& event);
    void Clear();

private:
    std::unordered_map<EVENT_TYPE, std::vector<EventHandler>> handlers_;
};

} // namespace mcx
