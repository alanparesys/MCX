#pragma once

#include <string>
#include <functional>
#include <thread>
#include <atomic>

namespace mcx {

using EventCallback = std::function<void(const std::string& jsonEvent)>;

class PaperBridge {
public:
    explicit PaperBridge(uint16_t port);
    ~PaperBridge();

    void Start();
    void Stop();
    void OnEvent(EventCallback callback);
    void SendAction(const std::string& jsonAction);

private:
    uint16_t port_;
    std::atomic<bool> running_{false};
    std::thread listenerThread_;
    EventCallback eventCallback_;
    
    void Listen();
};

} // namespace mcx
