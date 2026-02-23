#pragma once

#include "mcx/export.hpp"
#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <chrono>

namespace mcx {

using BackupCallback = std::function<void(bool success, const std::string& path)>;

class MCX_API BackupManager {
public:
    void SetWorldPath(const std::string& path);
    void SetBackupInterval(std::chrono::minutes interval);
    void SetMaxBackups(size_t maxBackups);
    
    void StartAutoBackup();
    void StopAutoBackup();
    void BackupNow(BackupCallback callback);
    
    [[nodiscard]] std::vector<std::string> ListBackups() const;
    void RestoreBackup(const std::string& backupPath);

private:
    void BackupLoop();
    std::string GenerateBackupName() const;
    void CleanupOldBackups();

    std::string worldPath_;
    std::string backupPath_{"backups/"};
    std::chrono::minutes interval_{30};
    size_t maxBackups_{10};
    std::atomic<bool> running_{false};
    std::thread backupThread_;
};

} // namespace mcx
