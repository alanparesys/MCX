#include "mcx/lua_runtime.hpp"
#include "mcx/log.hpp"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <filesystem>
#include <fstream>

namespace mcx {

LuaRuntime::LuaRuntime() : L_(luaL_newstate()) {
    if (!L_) {
        throw std::runtime_error("Failed to create Lua state");
    }
    luaL_openlibs(L_);
    RegisterEventBindings();
}

LuaRuntime::~LuaRuntime() {
    if (L_) {
        lua_close(L_);
    }
}

void LuaRuntime::RegisterEventBindings() {
    lua_newtable(L_);
    
    lua_pushcfunction(L_, [](lua_State* L) -> int {
        const char* msg = luaL_checkstring(L, 1);
        log::Info(std::string("[Lua] ") + msg);
        return 0;
    });
    lua_setfield(L_, -2, "log");
    
    lua_setglobal(L_, "MCX");
}

void LuaRuntime::ExecuteScript(const std::string& path) {
    if (luaL_dofile(L_, path.c_str()) != LUA_OK) {
        log::Error("Lua error: " + std::string(lua_tostring(L_, -1)));
        lua_pop(L_, 1);
    }
}

void LuaRuntime::LoadScripts(const std::string& rootPath) {
    if (!std::filesystem::exists(rootPath)) {
        log::Warn("Script root not found: " + rootPath);
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(rootPath)) {
        if (entry.path().extension() == ".lua") {
            log::Info("Loading Lua script: " + entry.path().string());
            ExecuteScript(entry.path().string());
        }
    }
}

ActionList LuaRuntime::HandleEvent(const Event& event) {
    ActionList actions;
    
    lua_getglobal(L_, "onEvent");
    if (!lua_isfunction(L_, -1)) {
        lua_pop(L_, 1);
        return actions;
    }
    
    switch (event.type) {
        case EVENT_TYPE::PLAYER_JOIN:
            lua_pushstring(L_, "player_join");
            break;
        case EVENT_TYPE::PLAYER_QUIT:
            lua_pushstring(L_, "player_quit");
            break;
        case EVENT_TYPE::CHAT:
            lua_pushstring(L_, "chat");
            break;
        default:
            lua_pushstring(L_, "unknown");
    }
    
    if (lua_pcall(L_, 1, 1, 0) != LUA_OK) {
        log::Error("Lua event handler error: " + std::string(lua_tostring(L_, -1)));
        lua_pop(L_, 1);
        return actions;
    }
    
    lua_pop(L_, 1);
    return actions;
}

std::unique_ptr<ScriptRuntime> CreateLuaRuntime() {
    return std::make_unique<LuaRuntime>();
}

} // namespace mcx
