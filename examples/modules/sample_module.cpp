#include <mcx/imodule.hpp>
#include <mcx/log.hpp>

class SampleModule : public mcx::IModule {
public:
    mcx::ModuleInfo GetInfo() const override {
        return {"SampleModule", "1.0.0", "MCX Team", 1};
    }
    
    bool OnLoad() override {
        mcx::log::Info("SampleModule loaded");
        return true;
    }
    
    void OnUnload() override {
        mcx::log::Info("SampleModule unloaded");
    }
    
    void OnEvent(const mcx::Event& event, mcx::ActionList& actions) override {
        if (event.type == mcx::EVENT_TYPE::CHAT && event.chat.has_value()) {
            const auto& chat = event.chat.value();
            if (chat.message == "!sample") {
                mcx::log::Info("SampleModule handled !sample command");
            }
        }
    }
};

extern "C" MCX_API mcx::IModule* CreateModule() {
    return new SampleModule();
}

extern "C" MCX_API void DestroyModule(mcx::IModule* mod) {
    delete mod;
}
