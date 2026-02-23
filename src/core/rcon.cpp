#include "mcx/rcon.hpp"
#include "mcx/log.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

namespace mcx {

RconClient::RconClient() = default;

RconClient::~RconClient() {
    Disconnect();
}

bool RconClient::Connect(const std::string& host, uint16_t port) {
    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd_ < 0) return false;
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
    
    if (::connect(socketFd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        close(socketFd_);
        socketFd_ = -1;
        return false;
    }
    
    connected_ = true;
    return true;
}

void RconClient::Disconnect() {
    if (socketFd_ >= 0) {
        close(socketFd_);
        socketFd_ = -1;
    }
    connected_ = false;
}

bool RconClient::Authenticate(const std::string& password) {
    RconPacket packet{0, nextRequestId_++, 3, password};
    packet.length = 10 + static_cast<int32_t>(packet.payload.size());
    
    if (!SendPacket(packet)) return false;
    
    auto response = ReceivePacket();
    return response.requestId != -1;
}

std::string RconClient::SendCommand(const std::string& command) {
    RconPacket packet{0, nextRequestId_++, 2, command};
    packet.length = 10 + static_cast<int32_t>(packet.payload.size());
    
    if (!SendPacket(packet)) return "";
    
    auto response = ReceivePacket();
    return response.payload;
}

bool RconClient::IsConnected() const {
    return connected_.load();
}

bool RconClient::SendPacket(const RconPacket& packet) {
    std::vector<uint8_t> data;
    data.reserve(14 + packet.payload.size());
    
    auto append = [&data](int32_t val) {
        data.push_back(val & 0xFF);
        data.push_back((val >> 8) & 0xFF);
        data.push_back((val >> 16) & 0xFF);
        data.push_back((val >> 24) & 0xFF);
    };
    
    append(packet.length);
    append(packet.requestId);
    append(packet.type);
    
    for (char c : packet.payload) {
        data.push_back(static_cast<uint8_t>(c));
    }
    data.push_back(0);
    data.push_back(0);
    
    return send(socketFd_, data.data(), data.size(), 0) == static_cast<ssize_t>(data.size());
}

RconPacket RconClient::ReceivePacket() {
    RconPacket packet{};
    
    uint8_t header[12];
    if (recv(socketFd_, header, 12, 0) != 12) {
        return packet;
    }
    
    packet.length = header[0] | (header[1] << 8) | (header[2] << 16) | (header[3] << 24);
    packet.requestId = header[4] | (header[5] << 8) | (header[6] << 16) | (header[7] << 24);
    packet.type = header[8] | (header[9] << 8) | (header[10] << 16) | (header[11] << 24);
    
    int payloadLen = packet.length - 8;
    if (payloadLen > 0) {
        std::vector<uint8_t> payload(payloadLen);
        recv(socketFd_, payload.data(), payloadLen, 0);
        packet.payload.assign(reinterpret_cast<char*>(payload.data()), payloadLen - 2);
    }
    
    return packet;
}

// RconServer implementation

RconServer::RconServer(uint16_t port) : port_(port) {}

RconServer::~RconServer() {
    Stop();
}

void RconServer::Start() {
    serverFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd_ < 0) return;
    
    int opt = 1;
    setsockopt(serverFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);
    
    if (bind(serverFd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        close(serverFd_);
        return;
    }
    
    listen(serverFd_, 5);
    running_ = true;
    acceptThread_ = std::thread(&RconServer::AcceptLoop, this);
    
    log::Info("RCON server started on port " + std::to_string(port_));
}

void RconServer::Stop() {
    running_ = false;
    if (serverFd_ >= 0) {
        close(serverFd_);
    }
    if (acceptThread_.joinable()) {
        acceptThread_.join();
    }
}

void RconServer::SetPassword(const std::string& password) {
    password_ = password;
}

void RconServer::OnCommand(std::function<void(const std::string&)> handler) {
    commandHandler_ = std::move(handler);
}

void RconServer::AcceptLoop() {
    while (running_) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(serverFd_, &readfds);
        
        timeval timeout{1, 0};
        if (select(serverFd_ + 1, &readfds, nullptr, nullptr, &timeout) > 0) {
            sockaddr_in clientAddr;
            socklen_t addrLen = sizeof(clientAddr);
            int clientFd = accept(serverFd_, (sockaddr*)&clientAddr, &addrLen);
            
            if (clientFd >= 0) {
                HandleClient(clientFd);
                close(clientFd);
            }
        }
    }
}

void RconServer::HandleClient(int clientFd) {
    // Simplified RCON handler
    log::Info("RCON client connected");
    
    char buffer[1024];
    ssize_t len = recv(clientFd, buffer, sizeof(buffer), 0);
    if (len > 0 && commandHandler_) {
        // Parse and execute command
        commandHandler_("help");
    }
}

} // namespace mcx
