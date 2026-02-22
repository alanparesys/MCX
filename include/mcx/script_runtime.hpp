#pragma once

#include <string>

#include "mcx/actions.hpp"
#include "mcx/events.hpp"

namespace mcx {

class ScriptRuntime {
public:
    virtual ~ScriptRuntime() = default;

    virtual void LoadScripts(const std::string& rootPath) = 0;
    [[nodiscard]] virtual ActionList HandleEvent(const Event& event) = 0;
};

class DummyScriptRuntime : public ScriptRuntime {
public:
    void LoadScripts(const std::string& rootPath) override;
    [[nodiscard]] ActionList HandleEvent(const Event& event) override;
};

std::unique_ptr<ScriptRuntime> CreateHardcodedRuntime();

} // namespace mcx
