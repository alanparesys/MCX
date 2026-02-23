#include "mcx/module_loader.hpp"
#include "mcx/log.hpp"

#include <algorithm>

namespace mcx {

ModuleLoader::~ModuleLoader() {
    UnloadAll();
}

bool ModuleLoader::LoadModule(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        log::Error("Module not found: " + path.string());
        return false;
    }

#ifdef _WIN32
    HMODULE handle = LoadLibraryW(path.wstring().c_str());
#else
    void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif

    if (!handle) {
        log::Error("Failed to load module: " + path.string());
        return false;
    }

#ifdef _WIN32
    auto createFunc = (CreateModuleFunc)GetProcAddress(handle, MCX_MODULE_CREATE_FUNC);
#else
    auto createFunc = (CreateModuleFunc)dlsym(handle, MCX_MODULE_CREATE_FUNC);
#endif

    if (!createFunc) {
        log::Error("Module missing CreateModule export: " + path.string());
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
        return false;
    }

    IModule* instance = createFunc();
    if (!instance) {
        log::Error("Module CreateModule returned null: " + path.string());
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
        return false;
    }

    ModuleInfo info = instance->GetInfo();
    if (info.apiVersion != MCX_MODULE_API_VERSION) {
        log::Error("Module API version mismatch: " + path.string());
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
        return false;
    }

    if (!instance->OnLoad()) {
        log::Error("Module OnLoad failed: " + path.string());
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
        return false;
    }

    auto mod = std::make_unique<LoadedModule>();
    mod->path = path.string();
    mod->instance = instance;
    mod->info = info;
    mod->handle = handle;

    {
        std::unique_lock lock(mutex_);
        modules_.push_back(std::move(mod));
    }

    log::Info("Module loaded: " + info.name + " v" + info.version);
    return true;
}

void ModuleLoader::UnloadModule(const std::string& name) {
    std::unique_lock lock(mutex_);
    
    auto it = std::find_if(modules_.begin(), modules_.end(),
        [&name](const auto& mod) { return mod->info.name == name; });
    
    if (it == modules_.end()) return;

    (*it)->instance->OnUnload();

#ifdef _WIN32
    FreeLibrary((*it)->handle);
#else
    dlclose((*it)->handle);
#endif

    modules_.erase(it);
    log::Info("Module unloaded: " + name);
}

void ModuleLoader::UnloadAll() {
    std::unique_lock lock(mutex_);
    
    for (auto& mod : modules_) {
        mod->instance->OnUnload();
#ifdef _WIN32
        FreeLibrary(mod->handle);
#else
        dlclose(mod->handle);
#endif
    }
    modules_.clear();
}

void ModuleLoader::DispatchEvent(const Event& event, ActionList& actions) {
    std::shared_lock lock(mutex_);
    
    for (const auto& mod : modules_) {
        try {
            mod->instance->OnEvent(event, actions);
        } catch (const std::exception& e) {
            log::Error("Module exception in " + mod->info.name + ": " + e.what());
        }
    }
}

std::vector<ModuleInfo> ModuleLoader::GetLoadedModules() const {
    std::shared_lock lock(mutex_);
    std::vector<ModuleInfo> result;
    for (const auto& mod : modules_) {
        result.push_back(mod->info);
    }
    return result;
}

} // namespace mcx
