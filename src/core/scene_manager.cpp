#include "mcx/scene_manager.hpp"
#include "mcx/log.hpp"

namespace mcx {

const std::string& SceneManager::GetCurrentScene() const {
    return currentScene_;
}

void SceneManager::LoadScene(const std::string& name) {
    if (currentScene_ == name) {
        log::Info("Scene '" + name + "' is already loaded");
        return;
    }

    const std::string previousScene = currentScene_;
    currentScene_ = name;

    if (previousScene.empty()) {
        log::Info("Loaded scene: " + name);
    } else {
        log::Info("Scene transition: " + previousScene +
                  " -> " + name);
    }
}

} // namespace mcx
