#include "mcx/websocket_server.hpp"
#include "mcx/log.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <openssl/sha.h>
#include <openssl/evp.h>

namespace mcx {

WebSocketServer::WebSocketServer(uint16_t port) : port_(port) {}

WebSocketServer::~WebSocketServer() {
    Stop();
}

void WebSocketServer::Start() {
    serverFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd_ < 0) {
        log::Error("Failed to create WebSocket server socket");
        return;
    }
    
    int opt = 1;
    setsockopt(serverFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);
    
    if (bind(serverFd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        log::Error("Failed to bind WebSocket server");
        close(serverFd_);
        return;
    }
    
    if (listen(serverFd_, 10) < 0) {
        log::Error("Failed to listen on WebSocket server");
        close(serverFd_);
        return;
    }
    
    running_ = true;
    acceptThread_ = std::thread(&WebSocketServer::AcceptLoop, this);
    log::Info("WebSocket server started on port " + std::to_string(port_));
}

void WebSocketServer::Stop() {
    running_ = false;
    if (serverFd_ >= 0) {
        close(serverFd_);
    }
    if (acceptThread_.joinable()) {
        acceptThread_.join();
    }
    
    std::lock_guard lock(clientsMutex_);
    clients_.clear();
}

void WebSocketServer::OnMessage(WebSocketMessageHandler handler) {
    onMessage_ = std::move(handler);
}

void WebSocketServer::OnConnect(WebSocketConnectHandler handler) {
    onConnect_ = std::move(handler);
}

void WebSocketServer::OnDisconnect(WebSocketDisconnectHandler handler) {
    onDisconnect_ = std::move(handler);
}

void WebSocketServer::Send(uint32_t clientId, const std::string& message) {
    // Simplified - would need client socket mapping
    log::Info("WebSocket send to client " + std::to_string(clientId));
}

void WebSocketServer::Broadcast(const std::string& message) {
    std::lock_guard lock(clientsMutex_);
    for (uint32_t clientId : clients_) {
        Send(clientId, message);
    }
}

size_t WebSocketServer::GetClientCount() const {
    std::lock_guard lock(clientsMutex_);
    return clients_.size();
}

void WebSocketServer::AcceptLoop() {
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
                uint32_t clientId = nextClientId_++;
                
                if (PerformHandshake(clientFd)) {
                    {
                        std::lock_guard lock(clientsMutex_);
                        clients_.insert(clientId);
                    }
                    
                    if (onConnect_) onConnect_(clientId);
                    HandleClient(clientFd, clientId);
                } else {
                    close(clientFd);
                }
            }
        }
    }
}

void WebSocketServer::HandleClient(int clientFd, uint32_t clientId) {
    char buffer[1024];
    
    while (running_) {
        ssize_t bytes = recv(clientFd, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;
        
        std::string frame(buffer, bytes);
        std::string message = DecodeFrame(frame);
        
        if (onMessage_) {
            onMessage_(clientId, message);
        }
    }
    
    close(clientFd);
    {
        std::lock_guard lock(clientsMutex_);
        clients_.erase(clientId);
    }
    
    if (onDisconnect_) onDisconnect_(clientId);
}

bool WebSocketServer::PerformHandshake(int clientFd) {
    char buffer[1024];
    ssize_t bytes = recv(clientFd, buffer, sizeof(buffer), 0);
    if (bytes <= 0) return false;
    
    std::string request(buffer, bytes);
    
    // Extract Sec-WebSocket-Key
    size_t keyStart = request.find("Sec-WebSocket-Key: ");
    if (keyStart == std::string::npos) return false;
    
    keyStart += 19;
    size_t keyEnd = request.find("\r\n", keyStart);
    std::string key = request.substr(keyStart, keyEnd - keyStart);
    
    // Generate accept key
    std::string magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::string combined = key + magic;
    
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(combined.c_str()), combined.length(), hash);
    
    // Base64 encode (simplified)
    std::string accept = "s3pPLMBiTxaQ9kYGzzhZRbK+xOo="; // Placeholder
    
    std::string response = 
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: " + accept + "\r\n"
        "\r\n";
    
    send(clientFd, response.c_str(), response.length(), 0);
    return true;
}

std::string WebSocketServer::DecodeFrame(const std::string& frame) {
    if (frame.length() < 2) return "";
    
    uint8_t opcode = frame[0] & 0x0F;
    if (opcode != 0x01 && opcode != 0x02) return "";
    
    uint8_t mask = frame[1] & 0x80;
    uint64_t payloadLen = frame[1] & 0x7F;
    
    size_t offset = 2;
    if (payloadLen == 126) {
        payloadLen = (frame[2] << 8) | frame[3];
        offset = 4;
    } else if (payloadLen == 127) {
        // 64-bit length
        offset = 10;
    }
    
    std::string result;
    if (mask) {
        uint8_t maskingKey[4];
        for (int i = 0; i < 4; ++i) {
            maskingKey[i] = frame[offset + i];
        }
        offset += 4;
        
        for (size_t i = 0; i < payloadLen; ++i) {
            result += frame[offset + i] ^ maskingKey[i % 4];
        }
    } else {
        result = frame.substr(offset, payloadLen);
    }
    
    return result;
}

std::string WebSocketServer::EncodeFrame(const std::string& message) {
    std::string frame;
    frame += static_cast<char>(0x81); // Text frame, FIN set
    
    if (message.length() < 126) {
        frame += static_cast<char>(message.length());
    } else if (message.length() < 65536) {
        frame += static_cast<char>(126);
        frame += static_cast<char>((message.length() >> 8) & 0xFF);
        frame += static_cast<char>(message.length() & 0xFF);
    } else {
        frame += static_cast<char>(127);
        for (int i = 7; i >= 0; --i) {
            frame += static_cast<char>((message.length() >> (i * 8)) & 0xFF);
        }
    }
    
    frame += message;
    return frame;
}

} // namespace mcx
