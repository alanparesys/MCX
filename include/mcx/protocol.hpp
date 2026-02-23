#pragma once

#include "mcx/export.hpp"
#include <string>
#include <vector>

namespace mcx {

namespace Protocol {
    constexpr uint8_t MAGIC = 0x4D;
    constexpr uint8_t VERSION = 1;
    
    enum class PacketType : uint8_t {
        HELLO = 0x01,
        HEARTBEAT = 0x02,
        PLAYER_JOIN = 0x10,
        PLAYER_QUIT = 0x11,
        CHAT_MESSAGE = 0x12,
        COMMAND = 0x13,
        ACTION_EXECUTE = 0x20,
        ACTION_RESULT = 0x21,
        ERROR = 0xFF
    };
}

struct McxPacket {
    Protocol::PacketType type;
    std::vector<uint8_t> data;
};

class MCX_API ProtocolHandler {
public:
    std::vector<uint8_t> Serialize(const McxPacket& packet);
    bool Deserialize(const uint8_t* data, size_t len, McxPacket& out);
    
    McxPacket CreatePlayerJoin(const std::string& playerId, const std::string& playerName);
    McxPacket CreatePlayerQuit(const std::string& playerId);
    McxPacket CreateChatMessage(const std::string& playerId, const std::string& message);
    McxPacket CreateAction(const std::string& actionType, const std::string& payload);
};

} // namespace mcx
