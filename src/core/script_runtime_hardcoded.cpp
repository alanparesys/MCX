#include "mcx/script_runtime_hardcoded.hpp"
#include "mcx/log.hpp"

namespace mcx {

class HardcodedScriptRuntime : public ScriptRuntime {
public:
    void LoadScripts(const std::string& rootPath) override {
        log::Info("HardcodedScriptRuntime.LoadScripts from: "
                  + rootPath);
    }

    [[nodiscard]] ActionList HandleEvent(
        const Event& event) override {
        ActionList actions{};

        if (event.type == EVENT_TYPE::CHAT && event.chat.has_value()) {
            const auto& chatEvent = event.chat.value();
            const auto& player = chatEvent.player;
            const auto& message = chatEvent.message;

            if (message == "!hello") {
                BroadcastAction action{};
                action.text = "[MCX] Hello from the hardcoded runtime, "
                              + player.name + "!";
                actions.push_back(action);
            } else if (message == "!scene test") {
                BroadcastAction info{};
                info.text = "[MCX] Loading test scene (hardcoded).";
                actions.push_back(info);

                LoadSceneAction load{};
                load.scene = "test";
                actions.push_back(load);
            }
        }

        return actions;
    }
};

std::unique_ptr<ScriptRuntime> CreateHardcodedRuntime() {
    return std::make_unique<HardcodedScriptRuntime>();
}

} // namespace mcx
