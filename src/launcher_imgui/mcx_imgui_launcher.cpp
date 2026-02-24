#include "mcx_imgui_launcher.hpp"
#include "../core/update_checker.hpp"
#include "../core/log.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#if defined(_WIN32)
    #include <windows.h>
#endif

MCXImGuiLauncher::MCXImGuiLauncher()
    : window_(nullptr)
    , io_(nullptr)
    , show_update_dialog_(false)
    , should_exit_(false)
    , auto_scroll_console_(true)
    , server_running_(false)
    , checking_update_(false)
    , update_progress_(0.0f)
    , update_available_(false)
    , update_checker_(std::make_unique<mcx::UpdateChecker>())
{
    LoadConfig();
}

MCXImGuiLauncher::~MCXImGuiLauncher() {
    Shutdown();
}

bool MCXImGuiLauncher::Initialize() {
    // Setup GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    // Create window
    window_ = glfwCreateWindow(1200, 800, "MCX Server Launcher", nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create GLFW window\n";
        return false;
    }
    
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable vsync
    
    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io_ = &ImGui::GetIO();
    io_->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io_->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    SetupStyle();
    
    // Initial console message
    AppendConsole("MCX Server Launcher " + std::string(VERSION) + " started", "green");
    AppendConsole("Press 'Check Update' to find latest version", "yellow");
    
    // Auto check update on startup
    if (config_.auto_update) {
        CheckForUpdates();
    }
    
    return true;
}

void MCXImGuiLauncher::SetupStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    
    // Dark theme
    colors[ImGuiCol_WindowBg]             = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]              = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_PopupBg]              = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_Border]               = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    colors[ImGuiCol_FrameBg]              = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive]        = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBg]              = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgActive]        = ImVec4(0.20f, 0.40f, 0.80f, 1.00f);
    colors[ImGuiCol_Button]               = ImVec4(0.20f, 0.50f, 0.90f, 1.00f);
    colors[ImGuiCol_ButtonHovered]        = ImVec4(0.25f, 0.60f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive]         = ImVec4(0.15f, 0.40f, 0.80f, 1.00f);
    colors[ImGuiCol_Header]               = ImVec4(0.20f, 0.40f, 0.80f, 0.50f);
    colors[ImGuiCol_HeaderHovered]        = ImVec4(0.25f, 0.50f, 0.90f, 0.80f);
    colors[ImGuiCol_HeaderActive]         = ImVec4(0.30f, 0.60f, 1.00f, 1.00f);
    colors[ImGuiCol_Tab]                  = ImVec4(0.18f, 0.35f, 0.70f, 0.86f);
    colors[ImGuiCol_TabHovered]           = ImVec4(0.25f, 0.50f, 0.90f, 0.80f);
    colors[ImGuiCol_TabActive]            = ImVec4(0.30f, 0.60f, 1.00f, 1.00f);
    colors[ImGuiCol_Text]                 = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]         = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
}

void MCXImGuiLauncher::Run() {
    while (!glfwWindowShouldClose(window_) && !should_exit_) {
        glfwPollEvents();
        
        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Docking setup
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        
        RenderUI();
        
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window_);
    }
}

void MCXImGuiLauncher::RenderUI() {
    RenderMainMenuBar();
    RenderConsole();
    RenderControlPanel();
    RenderConfigPanel();
    RenderUpdateDialog();
}

void MCXImGuiLauncher::RenderMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Start Server", "Ctrl+S", false, !server_running_)) StartServer();
            if (ImGui::MenuItem("Stop Server", "Ctrl+T", false, server_running_)) StopServer();
            ImGui::Separator();
            if (ImGui::MenuItem("Check for Updates")) CheckForUpdates();
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Ctrl+Q")) should_exit_ = true;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Clear Console")) console_lines_.clear();
            if (ImGui::MenuItem("Save Config")) SaveConfig();
            if (ImGui::MenuItem("Load Config")) LoadConfig();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                show_update_dialog_ = true;
            }
            ImGui::EndMenu();
        }
        
        // Right-aligned status
        float statusWidth = ImGui::CalcTextSize("Server: Stopped | v0.3.1").x;
        ImGui::SameLine(ImGui::GetWindowWidth() - statusWidth - 20);
        
        if (server_running_) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "● Running | %s", VERSION);
        } else {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "● Stopped | %s", VERSION);
        }
        
        ImGui::EndMainMenuBar();
    }
}

void MCXImGuiLauncher::RenderConsole() {
    ImGui::Begin("Console", nullptr, ImGuiWindowFlags_None);
    
    // Console toolbar
    if (ImGui::Button("Clear")) console_lines_.clear();
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &auto_scroll_console_);
    ImGui::Separator();
    
    // Console output
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -30), true, ImGuiWindowFlags_HorizontalScrollbar);
    
    for (const auto& line : console_lines_) {
        ImVec4 color;
        if (line.color == "red") color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
        else if (line.color == "green") color = ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
        else if (line.color == "yellow") color = ImVec4(1.0f, 1.0f, 0.3f, 1.0f);
        else color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        ImGui::TextColored(color, "[%05.2f] %s", line.timestamp, line.text.c_str());
    }
    
    if (auto_scroll_console_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }
    
    ImGui::EndChild();
    
    ImGui::Separator();
    
    // Command input (placeholder)
    static char command[256] = "";
    ImGui::InputText("Command", command, IM_ARRAYSIZE(command));
    ImGui::SameLine();
    if (ImGui::Button("Send") && strlen(command) > 0) {
        AppendConsole(std::string("> ") + command, "yellow");
        command[0] = '\0';
    }
    
    ImGui::End();
}

void MCXImGuiLauncher::RenderControlPanel() {
    ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_None);
    
    // Server control buttons
    ImVec2 buttonSize(100, 40);
    
    ImGui::PushStyleColor(ImGuiCol_Button, server_running_ ? ImVec4(0.5f, 0.5f, 0.5f, 1.0f) : ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
    if (ImGui::Button("▶ Start", buttonSize) && !server_running_) {
        StartServer();
    }
    ImGui::PopStyleColor();
    
    ImGui::SameLine();
    
    ImGui::PushStyleColor(ImGuiCol_Button, !server_running_ ? ImVec4(0.5f, 0.5f, 0.5f, 1.0f) : ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    if (ImGui::Button("⏹ Stop", buttonSize) && server_running_) {
        StopServer();
    }
    ImGui::PopStyleColor();
    
    ImGui::SameLine();
    
    if (ImGui::Button("🔄 Restart", buttonSize)) {
        StopServer();
        StartServer();
    }
    
    ImGui::Separator();
    
    // Update section
    if (checking_update_) {
        ImGui::Text("Checking for updates...");
        ImGui::ProgressBar(update_progress_, ImVec2(-1, 0), nullptr);
    } else if (update_available_) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "⚠ Update available: %s", latest_version_.c_str());
        if (ImGui::Button("Download Update")) {
#if defined(_WIN32)
            system(("start \"\" \"" + download_url_ + "\"").c_str());
#else
            system(("xdg-open \"" + download_url_ + "\"").c_str());
#endif
        }
    } else {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✓ Up to date (%s)", VERSION);
    }
    
    ImGui::SameLine(ImGui::GetWindowWidth() - 120);
    if (ImGui::Button("Check Update", ImVec2(100, 30)) && !checking_update_) {
        CheckForUpdates();
    }
    
    ImGui::Separator();
    
    // Quick stats
    ImGui::Text("Players: 0/%d", config_.max_players);
    ImGui::Text("Port: %d", config_.port);
    ImGui::Text("Mode: %s", config_.demo_mode ? "Demo" : "Production");
    
    ImGui::End();
}

void MCXImGuiLauncher::RenderConfigPanel() {
    ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_None);
    
    if (ImGui::BeginTabBar("ConfigTabs")) {
        if (ImGui::BeginTabItem("General")) {
            ImGui::InputText("Server Name", config_.server_name, IM_ARRAYSIZE(config_.server_name));
            ImGui::InputText("Bind IP", config_.bind_ip, IM_ARRAYSIZE(config_.bind_ip));
            ImGui::InputInt("Port", &config_.port);
            ImGui::InputInt("Max Players", &config_.max_players);
            ImGui::Checkbox("Demo Mode", &config_.demo_mode);
            ImGui::Checkbox("Auto-check Updates", &config_.auto_update);
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("RCON")) {
            ImGui::Checkbox("Enable RCON", &config_.rcon_enabled);
            ImGui::InputInt("RCON Port", &config_.rcon_port);
            ImGui::InputText("RCON Password", config_.rcon_password, IM_ARRAYSIZE(config_.rcon_password), ImGuiInputTextFlags_Password);
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::Separator();
    
    if (ImGui::Button("Save Config", ImVec2(120, 30))) {
        SaveConfig();
        AppendConsole("Configuration saved!", "green");
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Config", ImVec2(120, 30))) {
        LoadConfig();
        AppendConsole("Configuration loaded!", "green");
    }
    
    ImGui::End();
}

void MCXImGuiLauncher::RenderUpdateDialog() {
    if (!show_update_dialog_) return;
    
    ImGui::OpenPopup("About MCX");
    
    if (ImGui::BeginPopupModal("About MCX", &show_update_dialog_, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("MCX Server Launcher %s", VERSION);
        ImGui::Separator();
        ImGui::Text("Minecraft C++ Experience Layer");
        ImGui::Text("A server-side scripting platform for Minecraft");
        ImGui::Separator();
        ImGui::Text("Built with Dear ImGui + GLFW + OpenGL");
        ImGui::Separator();
        ImGui::Text("License: MIT");
        ImGui::Text("GitHub: github.com/alanparesys/MCX");
        
        if (ImGui::Button("Close", ImVec2(120, 0))) {
            show_update_dialog_ = false;
        }
        
        ImGui::EndPopup();
    }
}

void MCXImGuiLauncher::StartServer() {
    if (server_running_) return;
    
    SaveConfig();
    server_running_ = true;
    AppendConsole("Starting MCX Server...", "yellow");
    
    // Simulate server start (in real implementation, launch actual process)
    server_thread_ = std::thread([this]() {
        // This would actually run the MCX server
        AppendConsole("Server started successfully!", "green");
        AppendConsole("Listening on port " + std::to_string(config_.port), "green");
        
        // Simulate some activity
        for (int i = 0; i < 5 && server_running_; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (server_running_) {
                AppendConsole("Heartbeat " + std::to_string(i + 1), "white");
            }
        }
    });
    server_thread_.detach();
}

void MCXImGuiLauncher::StopServer() {
    if (!server_running_) return;
    
    server_running_ = false;
    AppendConsole("Stopping server...", "yellow");
    AppendConsole("Server stopped", "green");
}

void MCXImGuiLauncher::CheckForUpdates() {
    if (checking_update_) return;
    
    checking_update_ = true;
    update_progress_ = 0.0f;
    update_available_ = false;
    AppendConsole("Checking for updates...", "yellow");
    
    // Run check in background
    std::thread([this]() {
        // Simulate progress
        for (int i = 0; i <= 100 && checking_update_; i += 5) {
            update_progress_ = i / 100.0f;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        // In real implementation, call update_checker_->CheckForUpdates()
        // For demo, simulate finding an update
        latest_version_ = "v0.4.0";
        download_url_ = "https://github.com/alanparesys/MCX/releases/latest";
        update_available_ = (latest_version_ != VERSION);
        
        if (update_available_) {
            AppendConsole("Update available: " + latest_version_, "yellow");
        } else {
            AppendConsole("Up to date!", "green");
        }
        
        checking_update_ = false;
    }).detach();
}

void MCXImGuiLauncher::AppendConsole(const std::string& text, const char* color) {
    ConsoleLine line;
    line.text = text;
    line.color = color;
    line.timestamp = ImGui::GetTime();
    console_lines_.push_back(line);
    
    // Keep only last 1000 lines
    if (console_lines_.size() > 1000) {
        console_lines_.erase(console_lines_.begin());
    }
}

void MCXImGuiLauncher::SaveConfig() {
    std::ofstream file("mcx-config.json");
    if (file.is_open()) {
        file << "{\n";
        file << "  \"serverName\": \"" << config_.server_name << "\",\n";
        file << "  \"bindIp\": \"" << config_.bind_ip << "\",\n";
        file << "  \"port\": " << config_.port << ",\n";
        file << "  \"maxPlayers\": " << config_.max_players << ",\n";
        file << "  \"demoMode\": " << (config_.demo_mode ? "true" : "false") << ",\n";
        file << "  \"autoUpdate\": " << (config_.auto_update ? "true" : "false") << ",\n";
        file << "  \"rconEnabled\": " << (config_.rcon_enabled ? "true" : "false") << ",\n";
        file << "  \"rconPort\": " << config_.rcon_port << ",\n";
        file << "  \"rconPassword\": \"" << config_.rcon_password << "\"\n";
        file << "}\n";
        file.close();
    }
}

void MCXImGuiLauncher::LoadConfig() {
    std::ifstream file("mcx-config.json");
    if (!file.is_open()) return;
    
    // Simple parsing (in production, use JSON library)
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("serverName") != std::string::npos) {
            sscanf(line.c_str(), " * \"serverName\": \"%[^\"]\"", config_.server_name);
        } else if (line.find("bindIp") != std::string::npos) {
            sscanf(line.c_str(), " * \"bindIp\": \"%[^\"]\"", config_.bind_ip);
        } else if (line.find("port\":") != std::string::npos) {
            sscanf(line.c_str(), " * \"port\": %d", &config_.port);
        } else if (line.find("maxPlayers") != std::string::npos) {
            sscanf(line.c_str(), " * \"maxPlayers\": %d", &config_.max_players);
        }
    }
    file.close();
}

void MCXImGuiLauncher::Shutdown() {
    StopServer();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window_);
    glfwTerminate();
}

// C interface
extern "C" int RunImGuiLauncher() {
    MCXImGuiLauncher launcher;
    
    if (!launcher.Initialize()) {
        return 1;
    }
    
    launcher.Run();
    return 0;
}

// Main entry point
int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    return RunImGuiLauncher();
}
