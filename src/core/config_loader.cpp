#include "mcx/config_loader.hpp"
#include "mcx/log.hpp"

#include <fstream>

#include "nlohmann/json.hpp"

namespace mcx {

std::optional<Config> LoadConfigFromFile(
    const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        log::Warn("Cannot open config file: " + path);
        return std::nullopt;
    }

    try {
        nlohmann::json json;
        file >> json;

        Config config{};

        if (json.contains("backendEndpoint")) {
            config.backendEndpoint =
                json["backendEndpoint"].get<std::string>();
        }

        if (json.contains("scriptRoot")) {
            config.scriptRoot =
                json["scriptRoot"].get<std::string>();
        }

        if (json.contains("maxPlayers")) {
            config.maxPlayers =
                json["maxPlayers"].get<int>();
        }

        if (json.contains("demoMode")) {
            config.demoMode =
                json["demoMode"].get<bool>();
        }

        log::Info("Loaded config from: " + path);
        return config;
    } catch (const std::exception& e) {
        log::Error("Failed to parse config: " +
                   std::string(e.what()));
        return std::nullopt;
    }
}

} // namespace mcx
