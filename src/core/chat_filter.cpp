#include "mcx/chat_filter.hpp"
#include "mcx/log.hpp"

namespace mcx {

void ChatFilter::AddProfanityFilter(const std::string& pattern) {
    profanityFilters_.emplace_back(pattern, std::regex::icase);
}

void ChatFilter::AddSpamPattern(const std::string& pattern) {
    spamPatterns_.emplace_back(pattern);
}

void ChatFilter::SetMaxMessageLength(size_t maxLen) {
    maxMessageLength_ = maxLen;
}

bool ChatFilter::IsClean(const std::string& message) const {
    if (message.length() > maxMessageLength_) return false;
    
    for (const auto& pattern : profanityFilters_) {
        if (std::regex_search(message, pattern)) return false;
    }
    return true;
}

std::string ChatFilter::Sanitize(const std::string& message) const {
    std::string result = message;
    for (const auto& pattern : profanityFilters_) {
        result = std::regex_replace(result, pattern, "***");
    }
    return result;
}

bool ChatFilter::IsSpam(const std::string& message, const std::string& playerId) {
    auto& timestamps = playerMessages_[playerId];
    auto now = std::chrono::steady_clock::now();
    
    // Remove messages older than 5 seconds
    timestamps.erase(
        std::remove_if(timestamps.begin(), timestamps.end(),
            [now](const auto& ts) { 
                return std::chrono::duration_cast<std::chrono::seconds>(now - ts).count() > 5; 
            }),
        timestamps.end());
    
    // Check rate limit (5 messages per 5 seconds)
    if (timestamps.size() >= 5) return true;
    
    timestamps.push_back(now);
    
    // Check spam patterns
    for (const auto& pattern : spamPatterns_) {
        if (std::regex_match(message, pattern)) return true;
    }
    
    return false;
}

} // namespace mcx
