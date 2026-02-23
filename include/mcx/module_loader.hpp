#pragma once

#include "mcx/imodule.hpp"
#include <vector>
#include <memory>
#include <string>
#include <filesystem>
#include <shared_mutex>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace mcx {

struct LoadedModule {
    std::string path;
    IModule* instance;
    ModuleInfo info;
#ifdef _WIN32
    HMODULE handle;
#else
    void* handle;
#endif
};

class MCX_API ModuleLoader {
public:
    ~ModuleLoader();
    
    bool LoadModule(const std::filesystem::path& path);
    void UnloadModule(const std::string& name);
    void UnloadAll();
    
    void DispatchEvent(const Event& event, ActionList& actions);
    std::vector<ModuleInfo> GetLoadedModules() const;
    
private:
    std::vector<std::unique_ptr<LoadedModule>> modules_;
    mutable std::shared_mutex mutex_;
};

} // namespace mcx
