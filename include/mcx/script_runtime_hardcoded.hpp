#pragma once

#include "mcx/script_runtime.hpp"
#include <memory>

namespace mcx {

std::unique_ptr<ScriptRuntime> CreateHardcodedRuntime();

} // namespace mcx
