#pragma once

#include "mcx/export.hpp"
#include <string>
#include <functional>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_set>

namespace mcx {

using WebSocketMessageHandler = std::function<void(uint32_t clientId, const std::string& message)>;
using WebSocketConnectHandler = std::function<void(uint32_t clientId)>;
using WebSocketDisconnectHandler = std::function<void(uint32_t clientId)>;

class MCX_API WebSocketServer {
public:
    explicit WebSocketServer(uint16_t port);
    ~WebSocketServer();
    
    void Start();
    void Stop();
    
    void OnMessage(WebSocketMessageHandler handler);
    void OnConnect(WebSocketConnectHandler handler);
    void OnDisconnect(WebSocketDisconnectHandler handler);
    
    void Send(uint32_t clientId, const std::string& message);
    void Broadcast(const std::string& message);
    
    [[nodiscard]] size_t GetClientCount() const;

private:
    void AcceptLoop();
    void HandleClient(int clientFd, uint32_t clientId);
    bool PerformHandshake(int clientFd);
    std::string DecodeFrame(const std::string& frame);
    std::string EncodeFrame(const std::string& message);
    
    uint16_t port_;
    std::atomic<bool> running_{false};
    std::thread acceptThread_;
    int serverFd_{-1};
    
    std::unordered_set<uint32_t> clients_;
    mutable std::mutex clientsMutex_;
    uint32_t nextClientId_{1};
    
    WebSocketMessageHandler onMessage_;
    WebSocketConnectHandler onConnect_;
    WebSocketDisconnectHandler onDisconnect_;
};

} // namespace mcx
