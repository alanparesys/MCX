#pragma once

#include "mcx/export.hpp"
#include "mcx/actions.hpp"
#include "mcx/events.hpp"
#include <string>
#include <cstdint>

namespace mcx {

struct ModuleInfo {
    std::string name;
    std::string version;
    std::string author;
    uint32_t apiVersion{1};
};

class MCX_API IModule {
public:
    virtual ~IModule() = default;
    
    virtual ModuleInfo GetInfo() const = 0;
    virtual bool OnLoad() = 0;
    virtual void OnUnload() = 0;
    virtual void OnEvent(const Event& event, ActionList& actions) = 0;
};

using CreateModuleFunc = IModule*(*)();
using DestroyModuleFunc = void(*)(IModule*);

} // namespace mcx

#define MCX_MODULE_API_VERSION 1
#define MCX_MODULE_CREATE_FUNC "CreateModule"
#define MCX_MODULE_DESTROY_FUNC "DestroyModule"
