#pragma once

#include "mcx/export.hpp"
#include <string>
#include <unordered_map>
#include <functional>

namespace mcx {

using Currency = int64_t;

struct Account {
    std::string playerId;
    Currency balance{0};
};

class MCX_API EconomyManager {
public:
    void CreateAccount(const std::string& playerId, Currency initialBalance = 0);
    void DeleteAccount(const std::string& playerId);
    
    [[nodiscard]] Currency GetBalance(const std::string& playerId) const;
    bool Deposit(const std::string& playerId, Currency amount);
    bool Withdraw(const std::string& playerId, Currency amount);
    bool Transfer(const std::string& fromId, const std::string& toId, Currency amount);
    
    void SetCurrencyName(const std::string& name);
    void SetStartingBalance(Currency amount);

private:
    std::unordered_map<std::string, Account> accounts_;
    std::string currencyName_{"Coins"};
    Currency startingBalance_{100};
};

} // namespace mcx
