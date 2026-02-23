#include "mcx/http_client.hpp"
#include "mcx/log.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

namespace mcx {

HttpClient::HttpClient() = default;

HttpClient::~HttpClient() {
    Stop();
}

void HttpClient::Start() {
    running_ = true;
    worker_ = std::thread(&HttpClient::WorkerLoop, this);
}

void HttpClient::Stop() {
    running_ = false;
    cv_.notify_all();
    if (worker_.joinable()) {
        worker_.join();
    }
}

void HttpClient::Get(const std::string& url, HttpCallback callback) {
    HttpRequest req;
    req.method = "GET";
    req.url = url;
    Request(req, callback);
}

void HttpClient::Post(const std::string& url, const std::string& body, HttpCallback callback) {
    HttpRequest req;
    req.method = "POST";
    req.url = url;
    req.body = body;
    req.headers.push_back({"Content-Length", std::to_string(body.length())});
    Request(req, callback);
}

void HttpClient::Request(const HttpRequest& req, HttpCallback callback) {
    {
        std::lock_guard lock(mutex_);
        queue_.push({req, callback});
    }
    cv_.notify_one();
}

void HttpClient::WorkerLoop() {
    while (running_) {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [this] { return !queue_.empty() || !running_; });
        
        if (!running_) break;
        
        auto pending = queue_.front();
        queue_.pop();
        lock.unlock();
        
        auto response = PerformRequest(pending.request);
        if (pending.callback) {
            pending.callback(response);
        }
    }
}

HttpResponse HttpClient::PerformRequest(const HttpRequest& req) {
    HttpResponse response;
    
    // Simple HTTP client - parse URL
    size_t protoEnd = req.url.find("://");
    if (protoEnd == std::string::npos) return response;
    
    size_t hostStart = protoEnd + 3;
    size_t pathStart = req.url.find('/', hostStart);
    
    std::string host = req.url.substr(hostStart, pathStart - hostStart);
    std::string path = (pathStart == std::string::npos) ? "/" : req.url.substr(pathStart);
    
    // Get port
    size_t portSep = host.find(':');
    int port = 80;
    if (portSep != std::string::npos) {
        port = std::stoi(host.substr(portSep + 1));
        host = host.substr(0, portSep);
    }
    
    // Resolve host
    hostent* server = gethostbyname(host.c_str());
    if (!server) return response;
    
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return response;
    
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);
    
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(sock);
        return response;
    }
    
    // Build request
    std::stringstream httpRequest;
    httpRequest << req.method << " " << path << " HTTP/1.1\r\n";
    httpRequest << "Host: " << host << "\r\n";
    httpRequest << "Connection: close\r\n";
    for (const auto& [key, val] : req.headers) {
        httpRequest << key << ": " << val << "\r\n";
    }
    httpRequest << "\r\n";
    httpRequest << req.body;
    
    std::string requestStr = httpRequest.str();
    send(sock, requestStr.c_str(), requestStr.length(), 0);
    
    // Read response
    char buffer[4096];
    std::string responseData;
    ssize_t bytesRead;
    while ((bytesRead = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        responseData += buffer;
    }
    close(sock);
    
    // Parse response
    size_t statusEnd = responseData.find("\r\n");
    if (statusEnd != std::string::npos) {
        std::string statusLine = responseData.substr(0, statusEnd);
        sscanf(statusLine.c_str(), "HTTP/%*s %d", &response.statusCode);
        
        size_t bodyStart = responseData.find("\r\n\r\n");
        if (bodyStart != std::string::npos) {
            response.body = responseData.substr(bodyStart + 4);
        }
        response.success = (response.statusCode >= 200 && response.statusCode < 300);
    }
    
    return response;
}

} // namespace mcx
