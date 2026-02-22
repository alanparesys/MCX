#pragma once

#include "mcx/events.hpp"

#include <map>
#include <optional>
#include <string>

namespace mcx {

// Player struct defined in events.hpp

// Tracks all connected players and their state.
class PlayerRegistry {
public:
    // Registers a new player. Overwrites if playerId already exists.
    void AddPlayer(const Player& player);

    // Removes a player by their ID.
    void RemovePlayer(const std::string& playerId);

    // Gets a player by ID. Returns std::nullopt if not found.
    [[nodiscard]] std::optional<Player> GetPlayer(
        const std::string& playerId) const;

    // Returns the number of online players.
    [[nodiscard]] size_t GetOnlineCount() const;

    // Updates the scene for a specific player.
    void UpdatePlayerScene(const std::string& playerId,
                           const std::string& scene);

private:
    std::map<std::string, Player> players_;
};

} // namespace mcx
