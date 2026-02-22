#pragma once

#include <fstream>
#include <string>

namespace mcx {

class FileLogger {
public:
    explicit FileLogger(const std::string& path);
    ~FileLogger();
    
    void Write(const std::string& message);
    bool IsOpen() const;
    
private:
    std::ofstream file_;
};

} // namespace mcx
