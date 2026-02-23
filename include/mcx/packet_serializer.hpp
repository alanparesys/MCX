#pragma once

#include "mcx/export.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>

namespace mcx {

class MCX_API PacketSerializer {
public:
    std::vector<uint8_t> SerializeString(const std::string& str);
    std::vector<uint8_t> SerializeUInt32(uint32_t value);
    std::vector<uint8_t> SerializeDouble(double value);
    
    std::string DeserializeString(const uint8_t* data, size_t& offset);
    uint32_t DeserializeUInt32(const uint8_t* data, size_t& offset);
    double DeserializeDouble(const uint8_t* data, size_t& offset);

private:
    void Append(std::vector<uint8_t>& buf, const void* data, size_t len);
};

} // namespace mcx
