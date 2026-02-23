#include "mcx/config_watcher.hpp"
#include "mcx/log.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#endif

namespace mcx {

ConfigWatcher::ConfigWatcher(std::string filepath) 
    : filepath_(std::move(filepath)) {}

ConfigWatcher::~ConfigWatcher() {
    Stop();
}

void ConfigWatcher::Start() {
    running_ = true;
    watcherThread_ = std::thread(&ConfigWatcher::WatchLoop, this);
}

void ConfigWatcher::Stop() {
    running_ = false;
    if (watcherThread_.joinable()) {
        watcherThread_.join();
    }
}

void ConfigWatcher::OnChange(ConfigChangeCallback callback) {
    callback_ = std::move(callback);
}

#ifdef _WIN32
void ConfigWatcher::WatchLoop() {
    HANDLE hDir = CreateFileA(
        filepath_.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL
    );
    
    if (hDir == INVALID_HANDLE_VALUE) {
        log::Error("Failed to watch config file");
        return;
    }
    
    char buffer[1024];
    DWORD bytesReturned;
    
    while (running_) {
        if (ReadDirectoryChangesW(hDir, buffer, sizeof(buffer), FALSE,
            FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesReturned, NULL, NULL)) {
            if (callback_) {
                callback_();
            }
        }
        Sleep(1000);
    }
    
    CloseHandle(hDir);
}
#else
void ConfigWatcher::WatchLoop() {
    int fd = inotify_init();
    if (fd < 0) {
        log::Error("Failed to init inotify");
        return;
    }

    int wd = inotify_add_watch(fd, filepath_.c_str(), IN_MODIFY);
    if (wd < 0) {
        log::Error("Failed to watch config file");
        close(fd);
        return;
    }

    char buffer[sizeof(inotify_event) + NAME_MAX + 1];
    
    while (running_) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        
        timeval timeout{1, 0};
        if (select(fd + 1, &fds, nullptr, nullptr, &timeout) > 0) {
            [[maybe_unused]] ssize_t len = read(fd, buffer, sizeof(buffer));
            if (callback_) {
                callback_();
            }
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}
#endif

} // namespace mcx
