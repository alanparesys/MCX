#pragma once

#include "mcx/script_runtime.hpp"
#include <memory>

struct lua_State;

namespace mcx {

class LuaRuntime : public ScriptRuntime {
public:
    LuaRuntime();
    ~LuaRuntime() override;

    void LoadScripts(const std::string& rootPath) override;
    [[nodiscard]] ActionList HandleEvent(const Event& event) override;

private:
    lua_State* L_;
    
    void RegisterEventBindings();
    void ExecuteScript(const std::string& path);
};

std::unique_ptr<ScriptRuntime> CreateLuaRuntime();

} // namespace mcx
