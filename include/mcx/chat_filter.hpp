#pragma once

#include "mcx/export.hpp"
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <chrono>

namespace mcx {

class MCX_API ChatFilter {
public:
    void AddProfanityFilter(const std::string& pattern);
    void AddSpamPattern(const std::string& pattern);
    void SetMaxMessageLength(size_t maxLen);
    
    [[nodiscard]] bool IsClean(const std::string& message) const;
    [[nodiscard]] std::string Sanitize(const std::string& message) const;
    [[nodiscard]] bool IsSpam(const std::string& message, const std::string& playerId);

private:
    std::vector<std::regex> profanityFilters_;
    std::vector<std::regex> spamPatterns_;
    size_t maxMessageLength_{256};
    std::unordered_map<std::string, std::vector<std::chrono::steady_clock::time_point>> playerMessages_;
};

} // namespace mcx
