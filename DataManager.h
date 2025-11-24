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
    int m_lastId = 0;

    const std::string FILE_USERS = "users.txt";
    const std::string FILE_RATES = "rates.tsv";
    const std::string FILE_TRANSACTIONS = "transactions.tsv";


    void loadUsers();
    void loadCurrencies();
    void loadTransactions();

    void saveUsers() const;
    void saveCurrencies() const;
    void saveTransactions() const;
    void appendTransactionToFile(const Transaction& tx) const;

    public:
    DataManager();
    ~DataManager();

    void saveAllData();

    User* authenticate(const std::string& username, const std::string& password);
    User* getUserByName(const std::string& username);
    void addUser(const std::string& username, const std::string& password);
    bool deleteUser(const std::string& username);
    bool changeUserPassword(const std::string& username, const std::string& newPass);
    const std::vector<User>& getUsers() const;

    const std::vector<Currency>& getCurrencies() const;
    Currency* getCurrencyByCode(const std::string& code);
    void addCurrency(const std::string& code, double buy, double sell);
    void updateCurrencyRate(const std::string& code, double buy, double sell);
    bool deleteCurrency(const std::string& code);

    void addTransaction(std::unique_ptr<Transaction> transaction);
    bool deleteTransaction(int id);
    bool editTransactionClient(int id, const std::string& name, const std::string& series, const std::string& num);
    const std::vector<std::unique_ptr<Transaction>>& getTransactions() const;

    std::vector<const Transaction*> searchTransactionsByClient(const std::string& partialName) const;
    void sortTransactionsByDate(bool ascending = true);
    void sortTransactionsByAmount(bool ascending = true);
    std::vector<const Transaction*> filterTransactionsByCurrency(const std::string& code) const;
    std::vector<const Transaction*> filterTransactionsByType(const std::string& type) const;
    std::vector<const Transaction*> filterTransactionsByCurrencyAndType(const std::string& code, const std::string& type) const;
    std::vector<const Transaction*> filterTransactionsByMinAmount(double minAmount) const;
};



#endif //DATAMANAGER_H
