#include "mcx/paper_bridge.hpp"
#include "mcx/log.hpp"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

namespace mcx {

PaperBridge::PaperBridge(uint16_t port) : port_(port) {}

PaperBridge::~PaperBridge() {
    Stop();
}

void PaperBridge::Start() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    running_ = true;
    listenerThread_ = std::thread(&PaperBridge::Listen, this);
    log::Info("PaperBridge listening on port " + std::to_string(port_));
}

void PaperBridge::Stop() {
    running_ = false;
    if (listenerThread_.joinable()) {
        listenerThread_.join();
    }
#ifdef _WIN32
    WSACleanup();
#endif
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
#ifdef _WIN32
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        log::Error("Bind failed");
#ifdef _WIN32
        closesocket(serverFd);
#else
        close(serverFd);
#endif
        return;
    }

    if (listen(serverFd, 3) < 0) {
        log::Error("Listen failed");
#ifdef _WIN32
        closesocket(serverFd);
#else
        close(serverFd);
#endif
        return;
    }

    while (running_) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET((unsigned int)serverFd, &readfds);
        
        timeval timeout{0, 100000};
        if (select(serverFd + 1, &readfds, nullptr, nullptr, &timeout) > 0) {
            int clientFd = accept(serverFd, nullptr, nullptr);
            if (clientFd >= 0) {
                char buffer[1024]{};
                recv(clientFd, buffer, sizeof(buffer), 0);
                if (eventCallback_) {
                    eventCallback_(buffer);
                }
#ifdef _WIN32
                closesocket(clientFd);
#else
                close(clientFd);
#endif
            }
        }
    }

#ifdef _WIN32
    closesocket(serverFd);
#else
    close(serverFd);
#endif
}

} // namespace mcx
