#pragma once

#include "mcx/export.hpp"
#include <string>
#include <functional>

namespace mcx {

struct ReleaseInfo {
    std::string version;
    std::string downloadUrl;
    std::string releaseNotes;
    bool updateAvailable{false};
};

class MCX_API UpdateChecker {
public:
    void CheckForUpdates(const std::string& currentVersion);
    void OnUpdateAvailable(std::function<void(const ReleaseInfo&)> callback);
    void DownloadUpdate(const std::string& url, const std::string& destination);
    
private:
    std::function<void(const ReleaseInfo&)> callback_;
    ReleaseInfo ParseGitHubResponse(const std::string& json);
};

} // namespace mcx
