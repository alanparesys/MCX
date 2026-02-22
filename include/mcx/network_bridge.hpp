#pragma once

#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

namespace mcx {

using NetworkCallback = std::function<void(const std::string& packet)>;

class NetworkBridge {
public:
    explicit NetworkBridge(uint16_t port);
    ~NetworkBridge();

    void Start();
    void Stop();
    bool IsConnected() const;
    void Send(const std::string& packet);
    void OnReceive(NetworkCallback callback);

private:
    uint16_t port_;
    std::atomic<bool> running_{false};
    std::atomic<bool> connected_{false};
    std::thread ioThread_;
    NetworkCallback callback_;
    
    std::queue<std::string> sendQueue_;
    std::mutex queueMutex_;
    
    void IoLoop();
    bool Connect();
    void Disconnect();
};

} // namespace mcx
