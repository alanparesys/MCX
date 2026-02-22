#include "mcx/script_runtime.hpp"

#include <iostream>

namespace mcx {

void DummyScriptRuntime::LoadScripts(const std::string& rootPath) {
    std::cout << "[MCX] DummyScriptRuntime.LoadScripts from: "
              << rootPath << std::endl;
}

ActionList DummyScriptRuntime::HandleEvent(const Event& event) {
    (void)event;
    return {};
}

} // namespace mcx
