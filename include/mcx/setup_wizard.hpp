#pragma once

#include "mcx/export.hpp"
#include <string>
#include <cstdint>

namespace mcx {

struct ServerConfig {
    std::string name{"MCX Server"};
    std::string bindIp{"0.0.0.0"};
    uint16_t port{25565};
    int maxPlayers{20};
    bool termsAccepted{false};
};

class MCX_API SetupWizard {
public:
    bool Run();
    [[nodiscard]] ServerConfig GetConfig() const;
    void GenerateConfigFile() const;

private:
    bool PromptTerms();
    std::string PromptString(const std::string& label, const std::string& defaultVal);
    uint16_t PromptPort(const std::string& label, uint16_t defaultVal);
    int PromptInt(const std::string& label, int min, int max, int defaultVal);
    bool ValidateIp(const std::string& ip);

    ServerConfig config_;
};

} // namespace mcx
