#pragma once

#include "mcx/export.hpp"
#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

namespace mcx {

struct RconPacket {
    int32_t length;
    int32_t requestId;
    int32_t type;
    std::string payload;
};

class MCX_API RconClient {
public:
    RconClient();
    ~RconClient();
    
    bool Connect(const std::string& host, uint16_t port);
    void Disconnect();
    bool Authenticate(const std::string& password);
    
    std::string SendCommand(const std::string& command);
    [[nodiscard]] bool IsConnected() const;

private:
    bool SendPacket(const RconPacket& packet);
    RconPacket ReceivePacket();
    
    int socketFd_{-1};
    std::atomic<bool> connected_{false};
    int32_t nextRequestId_{1};
};

class MCX_API RconServer {
public:
    explicit RconServer(uint16_t port);
    ~RconServer();
    
    void Start();
    void Stop();
    void SetPassword(const std::string& password);
    void OnCommand(std::function<void(const std::string&)> handler);

private:
    void AcceptLoop();
    void HandleClient(int clientFd);
    
    uint16_t port_;
    std::string password_;
    std::atomic<bool> running_{false};
    std::thread acceptThread_;
    int serverFd_{-1};
    std::function<void(const std::string&)> commandHandler_;
};

} // namespace mcx
