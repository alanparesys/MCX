#include "mcx/event_bus.hpp"

namespace mcx {

void EventBus::Subscribe(EVENT_TYPE type, EventHandler handler) {
    handlers_[type].push_back(std::move(handler));
}

void EventBus::Publish(const Event& event) {
    auto it = handlers_.find(event.type);
    if (it != handlers_.end()) {
        for (const auto& handler : it->second) {
            handler(event);
        }
    }
}

void EventBus::Clear() {
    handlers_.clear();
}

} // namespace mcx
