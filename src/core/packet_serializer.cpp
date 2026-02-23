#include "mcx/packet_serializer.hpp"

namespace mcx {

std::vector<uint8_t> PacketSerializer::SerializeString(const std::string& str) {
    std::vector<uint8_t> result;
    auto len = static_cast<uint32_t>(str.size());
    Append(result, &len, sizeof(len));
    Append(result, str.data(), str.size());
    return result;
}

std::vector<uint8_t> PacketSerializer::SerializeUInt32(uint32_t value) {
    std::vector<uint8_t> result;
    Append(result, &value, sizeof(value));
    return result;
}

std::vector<uint8_t> PacketSerializer::SerializeDouble(double value) {
    std::vector<uint8_t> result;
    Append(result, &value, sizeof(value));
    return result;
}

std::string PacketSerializer::DeserializeString(const uint8_t* data, size_t& offset) {
    uint32_t len;
    std::memcpy(&len, data + offset, sizeof(len));
    offset += sizeof(len);
    
    std::string result(reinterpret_cast<const char*>(data + offset), len);
    offset += len;
    return result;
}

uint32_t PacketSerializer::DeserializeUInt32(const uint8_t* data, size_t& offset) {
    uint32_t value;
    std::memcpy(&value, data + offset, sizeof(value));
    offset += sizeof(value);
    return value;
}

double PacketSerializer::DeserializeDouble(const uint8_t* data, size_t& offset) {
    double value;
    std::memcpy(&value, data + offset, sizeof(value));
    offset += sizeof(value);
    return value;
}

void PacketSerializer::Append(std::vector<uint8_t>& buf, const void* data, size_t len) {
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    buf.insert(buf.end(), bytes, bytes + len);
}

} // namespace mcx
