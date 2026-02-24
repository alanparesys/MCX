#include "mcx/update_checker.hpp"
#include "mcx/log.hpp"
#include "mcx/http_client.hpp"

#include <sstream>
#include <thread>
#include <cstdlib>

namespace mcx {

void UpdateChecker::CheckForUpdates(const std::string& currentVersion) {
    HttpClient client;
    client.Start();
    
    // GitHub API for latest release
    const std::string apiUrl = "https://api.github.com/repos/alanparesys/MCX/releases/latest";
    
    client.Get(apiUrl, [this, currentVersion](const HttpResponse& response) {
        if (response.success && response.statusCode == 200) {
            auto info = ParseGitHubResponse(response.body);
            
            // Simple version comparison (v0.1.0 vs v0.1.1)
            if (info.version > currentVersion) {
                info.updateAvailable = true;
                log::Info("Update available: " + info.version + " (current: " + currentVersion + ")");
                
                if (callback_) {
                    callback_(info);
                }
            } else {
                log::Info("MCX is up to date (" + currentVersion + ")");
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
    
    size_t urlPos = json.find("\"browser_download_url\":\"");
    if (urlPos != std::string::npos) {
        size_t start = urlPos + 26;
        size_t end = json.find("\"", start);
        if (end != std::string::npos) {
            info.downloadUrl = json.substr(start, end - start);
        }
    }
    
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
