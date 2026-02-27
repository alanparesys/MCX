#include "mcx/update_checker.hpp"
#include "mcx/log.hpp"
#include "mcx/http_client.hpp"

#include <sstream>
#include <thread>
#include <cstdlib>
#include <vector>

namespace {

std::vector<int> ParseVersionParts(std::string version) {
    if (!version.empty() && (version[0] == 'v' || version[0] == 'V')) {
        version.erase(version.begin());
    }

    std::vector<int> parts;
    std::stringstream ss(version);
    std::string token;
    while (std::getline(ss, token, '.')) {
        try {
            parts.push_back(std::stoi(token));
        } catch (...) {
            parts.push_back(0);
        }
    }
    return parts;
}

bool IsVersionNewer(const std::string& candidate, const std::string& current) {
    auto a = ParseVersionParts(candidate);
    auto b = ParseVersionParts(current);
    const size_t n = std::max(a.size(), b.size());
    a.resize(n, 0);
    b.resize(n, 0);

    for (size_t i = 0; i < n; ++i) {
        if (a[i] > b[i]) return true;
        if (a[i] < b[i]) return false;
    }
    return false;
}

std::string PickPreferredAssetUrl(const std::string& json) {
    std::vector<std::string> urls;
    size_t pos = 0;
    const std::string key = "\"browser_download_url\":\"";

    while ((pos = json.find(key, pos)) != std::string::npos) {
        size_t start = pos + key.size();
        size_t end = json.find("\"", start);
        if (end == std::string::npos) break;
        urls.push_back(json.substr(start, end - start));
        pos = end + 1;
    }

#if defined(_WIN32)
    for (const auto& u : urls) {
        if (u.find("windows") != std::string::npos && u.find(".zip") != std::string::npos) return u;
    }
#elif defined(__linux__)
    for (const auto& u : urls) {
        if (u.find("linux") != std::string::npos && (u.find(".tar.gz") != std::string::npos || u.find(".tgz") != std::string::npos)) return u;
    }
#endif

    return urls.empty() ? std::string{} : urls.front();
}

} // namespace

namespace mcx {

void UpdateChecker::CheckForUpdates(const std::string& currentVersion) {
    HttpClient client;
    client.Start();
    
    // GitHub API for latest release
    const std::string apiUrl = "https://api.github.com/repos/alanparesys/MCX/releases/latest";
    
    client.Get(apiUrl, [this, currentVersion](const HttpResponse& response) {
        if (response.success && response.statusCode == 200) {
            auto info = ParseGitHubResponse(response.body);
            
            info.updateAvailable = IsVersionNewer(info.version, currentVersion);
            if (info.updateAvailable) {
                log::Info("Update available: " + info.version + " (current: " + currentVersion + ")");
            } else {
                log::Info("MCX is up to date (" + currentVersion + ")");
            }

            if (callback_) {
                callback_(info);
            }
        } else {
            log::Warn("Could not check for updates (status: " + std::to_string(response.statusCode) + ")");
        }
    });
    
    // Give HTTP request time to complete
    std::this_thread::sleep_for(std::chrono::seconds(2));
    client.Stop();
}

void UpdateChecker::OnUpdateAvailable(std::function<void(const ReleaseInfo&)> callback) {
    callback_ = std::move(callback);
}

void UpdateChecker::DownloadUpdate(const std::string& url, const std::string& destination) {
    log::Info("Downloading update from: " + url);
    
    HttpClient client;
    client.Start();
    
    client.Get(url, [destination](const HttpResponse& response) {
        if (response.success) {
            // In real implementation, save binary to file
            log::Info("Download complete! Save to: " + destination);
            log::Info("Please extract and replace your current mcx executable");
        } else {
            log::Error("Download failed: " + std::to_string(response.statusCode));
        }
    });
    
    std::this_thread::sleep_for(std::chrono::seconds(5));
    client.Stop();
}

ReleaseInfo UpdateChecker::ParseGitHubResponse(const std::string& json) {
    ReleaseInfo info;
    
    // Simple string search parsing (more reliable than regex for this)
    size_t tagPos = json.find("\"tag_name\":\"");
    if (tagPos != std::string::npos) {
        size_t start = tagPos + 12;
        size_t end = json.find("\"", start);
        if (end != std::string::npos) {
            info.version = json.substr(start, end - start);
        }
    }
    
    info.downloadUrl = PickPreferredAssetUrl(json);
    
    size_t bodyPos = json.find("\"body\":\"");
    if (bodyPos != std::string::npos) {
        size_t start = bodyPos + 8;
        size_t end = json.find("\"", start);
        if (end != std::string::npos) {
            info.releaseNotes = json.substr(start, end - start);
        }
    }
    
    return info;
}

} // namespace mcx
