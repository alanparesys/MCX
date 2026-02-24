#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <atomic>

struct GLFWwindow;
struct ImGuiIO;

namespace mcx {
    class UpdateChecker;
}

class MCXImGuiLauncher {
public:
    MCXImGuiLauncher();
    ~MCXImGuiLauncher();
    
    bool Initialize();
    void Run();
    void Shutdown();
    
private:
    void SetupImGui();
    void SetupStyle();
    void RenderUI();
    void RenderMainMenuBar();
    void RenderConsole();
    void RenderControlPanel();
    void RenderConfigPanel();
    void RenderUpdateDialog();
    
    void StartServer();
    void StopServer();
    void CheckForUpdates();
    void AppendConsole(const std::string& text, const char* color = "white");
    void SaveConfig();
    void LoadConfig();
    
    // GLFW/ImGui
    GLFWwindow* window_;
    ImGuiIO* io_;
    bool show_update_dialog_;
    bool should_exit_;
    
    // Console buffer
    struct ConsoleLine {
        std::string text;
        std::string color;
        float timestamp;
    };
    std::vector<ConsoleLine> console_lines_;
    bool auto_scroll_console_;
    
    // Server state
    std::atomic<bool> server_running_;
    std::atomic<bool> checking_update_;
    float update_progress_;
    std::thread server_thread_;
    
    // Update info
    std::string latest_version_;
    std::string download_url_;
    bool update_available_;
    
    // Config
    struct Config {
        char server_name[128] = "MCX Server";
        char bind_ip[64] = "0.0.0.0";
        int port = 25565;
        int max_players = 50;
        bool demo_mode = true;
        bool auto_update = true;
        bool rcon_enabled = true;
        int rcon_port = 25575;
        char rcon_password[128] = "mcx-password-123";
    } config_;
    
    // Update checker
    std::unique_ptr<mcx::UpdateChecker> update_checker_;
    
    static constexpr const char* VERSION = "v0.3.1";
};

// C interface for main
extern "C" int RunImGuiLauncher();
