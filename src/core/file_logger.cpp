#include "mcx/file_logger.hpp"

#include <chrono>
#include <iomanip>

namespace mcx {

FileLogger::FileLogger(const std::string& path) 
    : file_(path, std::ios::app) {}

FileLogger::~FileLogger() = default;

void FileLogger::Write(const std::string& message) {
    if (!file_.is_open()) return;
    
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    file_ << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
          << " " << message << '\n';
}

bool FileLogger::IsOpen() const {
    return file_.is_open();
}

} // namespace mcx
