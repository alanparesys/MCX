#include "mcx/paper_bridge.hpp"
#include "mcx/log.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace mcx {

PaperBridge::PaperBridge(uint16_t port) : port_(port) {}

PaperBridge::~PaperBridge() {
    Stop();
}

void PaperBridge::Start() {
    running_ = true;
    listenerThread_ = std::thread(&PaperBridge::Listen, this);
    log::Info("PaperBridge listening on port " + std::to_string(port_));
}

void PaperBridge::Stop() {
    running_ = false;
    if (listenerThread_.joinable()) {
        listenerThread_.join();
    }
}

void PaperBridge::OnEvent(EventCallback callback) {
    eventCallback_ = std::move(callback);
}

void PaperBridge::SendAction(const std::string& jsonAction) {
    log::Info("PaperBridge send: " + jsonAction);
}

void PaperBridge::Listen() {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        log::Error("Failed to create socket");
        return;
    }

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        log::Error("Bind failed");
        close(serverFd);
        return;
    }

    if (listen(serverFd, 3) < 0) {
        log::Error("Listen failed");
        close(serverFd);
        return;
    }

    while (running_) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(serverFd, &readfds);
        
        timeval timeout{0, 100000};
        if (select(serverFd + 1, &readfds, nullptr, nullptr, &timeout) > 0) {
            int clientFd = accept(serverFd, nullptr, nullptr);
            if (clientFd >= 0) {
                char buffer[1024]{};
                read(clientFd, buffer, sizeof(buffer));
                if (eventCallback_) {
                    eventCallback_(buffer);
                }
                close(clientFd);
            }
        }
    }

    close(serverFd);
}

} // namespace mcx
