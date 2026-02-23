#include "mcx/economy_manager.hpp"
#include "mcx/log.hpp"

namespace mcx {

void EconomyManager::CreateAccount(const std::string& playerId, Currency initialBalance) {
    accounts_[playerId] = {playerId, initialBalance};
    log::Info("Created economy account for " + playerId);
}

void EconomyManager::DeleteAccount(const std::string& playerId) {
    accounts_.erase(playerId);
}

Currency EconomyManager::GetBalance(const std::string& playerId) const {
    auto it = accounts_.find(playerId);
    if (it != accounts_.end()) {
        return it->second.balance;
    }
    return 0;
}

bool EconomyManager::Deposit(const std::string& playerId, Currency amount) {
    if (amount <= 0) return false;
    
    auto it = accounts_.find(playerId);
    if (it == accounts_.end()) {
        CreateAccount(playerId, amount);
    } else {
        it->second.balance += amount;
    }
    return true;
}

bool EconomyManager::Withdraw(const std::string& playerId, Currency amount) {
    if (amount <= 0) return false;
    
    auto it = accounts_.find(playerId);
    if (it == accounts_.end() || it->second.balance < amount) {
        return false;
    }
    
    it->second.balance -= amount;
    return true;
}

bool EconomyManager::Transfer(const std::string& fromId, const std::string& toId, Currency amount) {
    if (!Withdraw(fromId, amount)) return false;
    Deposit(toId, amount);
    log::Info("Transferred " + std::to_string(amount) + " " + currencyName_ + 
              " from " + fromId + " to " + toId);
    return true;
}

void EconomyManager::SetCurrencyName(const std::string& name) {
    currencyName_ = name;
}

void EconomyManager::SetStartingBalance(Currency amount) {
    startingBalance_ = amount;
}

} // namespace mcx
