#pragma once

#include "mcx/export.hpp"
#include <string>
#include <functional>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace mcx {

struct HttpRequest {
    std::string method{"GET"};
    std::string url;
    std::vector<std::pair<std::string, std::string>> headers;
    std::string body;
};

struct HttpResponse {
    int statusCode{0};
    std::string body;
    std::vector<std::pair<std::string, std::string>> headers;
    bool success{false};
};

using HttpCallback = std::function<void(const HttpResponse&)>;

class MCX_API HttpClient {
public:
    HttpClient();
    ~HttpClient();
    
    void Get(const std::string& url, HttpCallback callback);
    void Post(const std::string& url, const std::string& body, HttpCallback callback);
    void Request(const HttpRequest& req, HttpCallback callback);
    
    void Start();
    void Stop();

private:
    void WorkerLoop();
    HttpResponse PerformRequest(const HttpRequest& req);
    
    struct PendingRequest {
        HttpRequest request;
        HttpCallback callback;
    };
    
    std::queue<PendingRequest> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread worker_;
    std::atomic<bool> running_{false};
};

} // namespace mcx
