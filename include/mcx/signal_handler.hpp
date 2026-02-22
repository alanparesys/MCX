#pragma once

#include <atomic>
#include <functional>

namespace mcx {

using ShutdownCallback = std::function<void()>;

void RegisterSignalHandler(ShutdownCallback callback);
bool ShouldShutdown();

} // namespace mcx
