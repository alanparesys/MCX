#pragma once

#include "mcx/actions.hpp"

namespace mcx {

class SceneManager;

// Applies actions to the server state.
// Uses SceneManager to handle scene transitions.
class ActionApplier {
public:
    explicit ActionApplier(SceneManager& sceneManager);

    void Apply(const ActionList& actions);

private:
    SceneManager& sceneManager_;
};

} // namespace mcx
