#include "mcx/backup_manager.hpp"
#include "mcx/log.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace mcx {

void BackupManager::SetWorldPath(const std::string& path) {
    worldPath_ = path;
}

void BackupManager::SetBackupInterval(std::chrono::minutes interval) {
    interval_ = interval;
}

void BackupManager::SetMaxBackups(size_t maxBackups) {
    maxBackups_ = maxBackups;
}

void BackupManager::StartAutoBackup() {
    running_ = true;
    backupThread_ = std::thread(&BackupManager::BackupLoop, this);
    log::Info("Auto-backup started");
}

void BackupManager::StopAutoBackup() {
    running_ = false;
    if (backupThread_.joinable()) {
        backupThread_.join();
    }
}

void BackupManager::BackupNow(BackupCallback callback) {
    try {
        if (!std::filesystem::exists(backupPath_)) {
            std::filesystem::create_directories(backupPath_);
        }
        
        std::string dest = backupPath_ + GenerateBackupName();
        std::filesystem::copy(worldPath_, dest, 
            std::filesystem::copy_options::recursive);
        
        CleanupOldBackups();
        
        log::Info("Backup created: " + dest);
        if (callback) callback(true, dest);
    } catch (const std::exception& e) {
        log::Error("Backup failed: " + std::string(e.what()));
        if (callback) callback(false, "");
    }
}

std::vector<std::string> BackupManager::ListBackups() const {
    std::vector<std::string> backups;
    if (!std::filesystem::exists(backupPath_)) return backups;
    
    for (const auto& entry : std::filesystem::directory_iterator(backupPath_)) {
        backups.push_back(entry.path().string());
    }
    return backups;
}

void BackupManager::RestoreBackup(const std::string& backupPath) {
    try {
        std::filesystem::remove_all(worldPath_);
        std::filesystem::copy(backupPath, worldPath_, 
            std::filesystem::copy_options::recursive);
        log::Info("Backup restored: " + backupPath);
    } catch (const std::exception& e) {
        log::Error("Restore failed: " + std::string(e.what()));
    }
}

void BackupManager::BackupLoop() {
    while (running_) {
        std::this_thread::sleep_for(interval_);
        if (running_) {
            BackupNow(nullptr);
        }
    }
}

std::string BackupManager::GenerateBackupName() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << "backup_" << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S");
    return ss.str();
}

void BackupManager::CleanupOldBackups() {
    auto backups = ListBackups();
    if (backups.size() <= maxBackups_) return;
    
    std::sort(backups.begin(), backups.end());
    
    size_t toDelete = backups.size() - maxBackups_;
    for (size_t i = 0; i < toDelete; ++i) {
        std::filesystem::remove_all(backups[i]);
        log::Info("Old backup removed: " + backups[i]);
    }
}

} // namespace mcx
