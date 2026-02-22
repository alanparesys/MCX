#pragma once

#include <string>

namespace mcx {

// Manages the current scene state for the server.
// Scenes represent distinct game modes or world states
// (lobby, arena, race, etc.).
class SceneManager {
public:
    // Returns the name of the currently loaded scene.
    // Returns empty string if no scene is active.
    [[nodiscard]] const std::string& GetCurrentScene() const;

    // Loads a new scene by name.
    // Updates the internal state and logs the transition.
    void LoadScene(const std::string& name);

private:
    std::string currentScene_;
};

} // namespace mcx
