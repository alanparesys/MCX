#include "mcx/network_bridge.hpp"
#include "mcx/log.hpp"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

namespace mcx {

NetworkBridge::NetworkBridge(uint16_t port) : port_(port) {}

NetworkBridge::~NetworkBridge() {
    Stop();
}

void NetworkBridge::Start() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    running_ = true;
    ioThread_ = std::thread(&NetworkBridge::IoLoop, this);
}

void NetworkBridge::Stop() {
    running_ = false;
    Disconnect();
    if (ioThread_.joinable()) {
        ioThread_.join();
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

bool NetworkBridge::IsConnected() const {
    return connected_.load();
}

void NetworkBridge::Send(const std::string& packet) {
    std::lock_guard<std::mutex> lock(queueMutex_);
    sendQueue_.push(packet);
}

void NetworkBridge::OnReceive(NetworkCallback callback) {
    callback_ = std::move(callback);
}

bool NetworkBridge::Connect() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port_);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    if (::connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return false;
    }

#ifndef _WIN32
    fcntl(sock, F_SETFL, O_NONBLOCK);
#endif
    connected_.store(true);
    return true;
}

void NetworkBridge::Disconnect() {
    connected_.store(false);
}

void NetworkBridge::IoLoop() {
    while (running_) {
        if (!connected_) {
            if (!Connect()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
        }
        
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            while (!sendQueue_.empty()) {
                sendQueue_.pop();
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

} // namespace mcx
