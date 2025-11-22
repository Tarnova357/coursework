//
// Created by SergiyT on 22.11.2025.
//

#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <memory>
#include <vector>

#include "Currency.h"
#include "Transaction.h"
#include "User.h"


class DataManager {
    private:
    std::vector<User> m_users;
    std::vector<Currency> m_currencies;
    std::vector<std::unique_ptr<Transaction>> m_transactions;

    const std::string FILE_USERS = "users.txt";
    const std::string FILE_RATES = "rates.tsv";
    const std::string FILE_TRANSACTIONS = "transactions.tsv";


    void loadUsers();
    void loadCurrencies();
    void loadTransactions();

    void saveUsers() const;
    void saveCurrencies() const;
    void saveTransactions() const;

    public:
    DataManager();
    ~DataManager();

    void saveAllData();


};



#endif //DATAMANAGER_H
