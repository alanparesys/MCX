#include "mcx/setup_wizard.hpp"
#include "mcx/log.hpp"

#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

namespace mcx {

bool SetupWizard::Run() {
    std::cout << "\n=== MCX Server Setup ===\n\n";
    
    if (!PromptTerms()) {
        std::cout << "Setup cancelled. Terms must be accepted to continue.\n";
        return false;
    }
    
    config_.name = PromptString("Server name", config_.name);
    config_.bindIp = PromptString("Bind IP", config_.bindIp);
    
    while (!ValidateIp(config_.bindIp)) {
        std::cout << "Invalid IP address.\n";
        config_.bindIp = PromptString("Bind IP", "0.0.0.0");
    }
    
    config_.port = PromptPort("Port", config_.port);
    config_.maxPlayers = PromptInt("Max players", 1, 1000, config_.maxPlayers);
    
    std::cout << "\n=== Configuration Summary ===\n";
    std::cout << "Server: " << config_.name << "\n";
    std::cout << "Bind: " << config_.bindIp << ":" << config_.port << "\n";
    std::cout << "Max players: " << config_.maxPlayers << "\n\n";
    
    std::string confirm = PromptString("Create server? (yes/no)", "yes");
    if (confirm != "yes") {
        std::cout << "Setup cancelled.\n";
        return false;
    }
    
    GenerateConfigFile();
    return true;
}

ServerConfig SetupWizard::GetConfig() const {
    return config_;
}

void SetupWizard::GenerateConfigFile() const {
    std::ofstream file("server.json");
    file << "{\n";
    file << "  \"name\": \"" << config_.name << "\",\n";
    file << "  \"bindIp\": \"" << config_.bindIp << "\",\n";
    file << "  \"port\": " << config_.port << ",\n";
    file << "  \"maxPlayers\": " << config_.maxPlayers << ",\n";
    file << "  \"termsAccepted\": true\n";
    file << "}\n";
    
    log::Info("Config saved to server.json");
}

bool SetupWizard::PromptTerms() {
    std::cout << "Terms of Service:\n";
    std::cout << "By running this server, you agree to the MCX license terms.\n\n";
    
    std::string response;
    std::cout << "Accept terms? (yes/no): ";
    std::getline(std::cin, response);
    
    std::transform(response.begin(), response.end(), response.begin(), ::tolower);
    config_.termsAccepted = (response == "yes");
    
    return config_.termsAccepted;
}

std::string SetupWizard::PromptString(const std::string& label, const std::string& defaultVal) {
    std::string response;
    std::cout << label << " [" << defaultVal << "]: ";
    std::getline(std::cin, response);
    return response.empty() ? defaultVal : response;
}

uint16_t SetupWizard::PromptPort(const std::string& label, uint16_t defaultVal) {
    while (true) {
        std::string response = PromptString(label, std::to_string(defaultVal));
        try {
            int port = std::stoi(response);
            if (port > 0 && port <= 65535) {
                return static_cast<uint16_t>(port);
            }
        } catch (...) {}
        std::cout << "Invalid port. Use 1-65535.\n";
    }
}

int SetupWizard::PromptInt(const std::string& label, int min, int max, int defaultVal) {
    while (true) {
        std::string response = PromptString(label, std::to_string(defaultVal));
        try {
            int val = std::stoi(response);
            if (val >= min && val <= max) {
                return val;
            }
        } catch (...) {}
        std::cout << "Invalid value. Use " << min << "-" << max << ".\n";
    }
}

bool SetupWizard::ValidateIp(const std::string& ip) {
    std::regex pattern("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$");
    std::smatch match;
    
    if (!std::regex_match(ip, match, pattern)) {
        return ip == "localhost" || ip == "*";
    }
    
    for (int i = 1; i <= 4; ++i) {
        int octet = std::stoi(match[i]);
        if (octet < 0 || octet > 255) return false;
    }
    
    return true;
}

} // namespace mcx
