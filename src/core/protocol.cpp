#include "mcx/protocol.hpp"
#include <cstdint>
#include <cstring>

namespace mcx {

std::vector<uint8_t> ProtocolHandler::Serialize(const McxPacket& packet) {
    std::vector<uint8_t> result;
    result.reserve(5 + packet.data.size());
    
    result.push_back(Protocol::MAGIC);
    result.push_back(Protocol::VERSION);
    result.push_back(static_cast<uint8_t>(packet.type));
    result.push_back(static_cast<uint8_t>(packet.data.size() & 0xFF));
    result.push_back(static_cast<uint8_t>((packet.data.size() >> 8) & 0xFF));
    
    result.insert(result.end(), packet.data.begin(), packet.data.end());
    return result;
}

bool ProtocolHandler::Deserialize(const uint8_t* data, size_t len, McxPacket& out) {
    if (len < 5) return false;
    if (data[0] != Protocol::MAGIC) return false;
    if (data[1] != Protocol::VERSION) return false;
    
    out.type = static_cast<Protocol::PacketType>(data[2]);
    uint16_t payloadLen = data[3] | (data[4] << 8);
    
    if (len < 5 + payloadLen) return false;
    
    out.data.assign(data + 5, data + 5 + payloadLen);
    return true;
}

McxPacket ProtocolHandler::CreatePlayerJoin(const std::string& playerId, const std::string& playerName) {
    McxPacket packet;
    packet.type = Protocol::PacketType::PLAYER_JOIN;
    packet.data.insert(packet.data.end(), playerId.begin(), playerId.end());
    packet.data.push_back(0);
    packet.data.insert(packet.data.end(), playerName.begin(), playerName.end());
    return packet;
}

McxPacket ProtocolHandler::CreatePlayerQuit(const std::string& playerId) {
    McxPacket packet;
    packet.type = Protocol::PacketType::PLAYER_QUIT;
    packet.data.insert(packet.data.end(), playerId.begin(), playerId.end());
    return packet;
}

McxPacket ProtocolHandler::CreateChatMessage(const std::string& playerId, const std::string& message) {
    McxPacket packet;
    packet.type = Protocol::PacketType::CHAT_MESSAGE;
    packet.data.insert(packet.data.end(), playerId.begin(), playerId.end());
    packet.data.push_back(0);
    packet.data.insert(packet.data.end(), message.begin(), message.end());
    return packet;
}

McxPacket ProtocolHandler::CreateAction(const std::string& actionType, const std::string& payload) {
    McxPacket packet;
    packet.type = Protocol::PacketType::ACTION_EXECUTE;
    packet.data.insert(packet.data.end(), actionType.begin(), actionType.end());
    packet.data.push_back(0);
    packet.data.insert(packet.data.end(), payload.begin(), payload.end());
    return packet;
}

} // namespace mcx
