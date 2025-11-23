//
// Created by SergiyT on 22.11.2025.
//

#include "DataManager.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "BuyTransaction.h"
#include "SellTransaction.h"


DataManager::DataManager() {
    loadUsers();
    loadCurrencies();
    loadTransactions();
}

DataManager::~DataManager() {
    saveAllData();
}

void DataManager::loadUsers() {
    std::ifstream file(FILE_USERS);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {continue;}
            size_t pos = line.find_first_of(":");
            if (pos == std::string::npos) {
                std::cerr <<"[Error] Skipped corrupted line in users.txt:" <<line << std::endl;
                continue;
            }
            std::string username = line.substr(0, pos);
            std::string password = line.substr(pos + 1);

            try {
                bool isAdmin = (username == "admin");
                m_users.emplace_back(username, password, isAdmin);
            } catch (const std::exception& e) {
                std::cerr << "[Error] User creation failed: " << e.what() << std::endl;
            }
            file.close();
        }

        bool adminExists = false;
        for (const auto& user : m_users) {
            if (user.getUsername() == "admin") {
                adminExists = true;
                break;
            }
        }

        if (!adminExists) {
            std::cout << "[Info] Admin not found. Creating default admin..." << std::endl;
            m_users.insert(m_users.begin(), User("admin", "admin123", true));
            saveUsers();
        }
    }
}

void DataManager::loadCurrencies() {
    std::ifstream file(FILE_RATES);
    if (!file.is_open()) return;

    file.imbue(std::locale::classic());

    Currency temp;
    while (file.peek() != EOF) {
        try {
            temp.deserialize(file);
            if (file.fail()) {
                file.clear();
                std::string dummy;
                std::getline(file, dummy);
                std::cerr << "[Warning] Corrupted data found in rates file. Skipped line: '"
                          << dummy << "'" << std::endl;
                continue;
            }
            m_currencies.emplace_back(temp);
        } catch (const std::exception& e) {
            std::cerr << "[Error] Error loading currency: " << e.what() << std::endl;
        }
    }
}

void DataManager::loadTransactions() {
    std::ifstream file(FILE_TRANSACTIONS);
    if (!file.is_open()) return;
    file.imbue(std::locale::classic());

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {continue;}
        std::stringstream ss(line);
        int id;
        std::string type;

        if (!(ss  >> id >> type)) {continue;}
        if (id > m_lastId) m_lastId = id;
        std::unique_ptr<Transaction> temp;

        try {
            if (type =="BUY") {
                temp = std::make_unique<BuyTransaction>();
            } else if (type =="SELL") {
                temp = std::make_unique<SellTransaction>();
            } else {
                std::cerr << "[Warning] Unknown transaction type: '" << type
                          << "' in line: '" << line << "'. Skipped." << std::endl;
                continue;
            }
            temp->setId(id);
            temp->deserialize(ss);
            m_transactions.emplace_back(std::move(temp));
        } catch (const std::exception& e) {
            std::cerr << "[Error] Transaction skipped: " << e.what() << std::endl;
        }
    }
}

void DataManager::saveUsers() const {
    std::ofstream usersFile(FILE_USERS);
    if (usersFile.is_open()) {
        for (const auto& user : m_users) {
            usersFile << user.getUsername() << ":" << user.getPassword() << "\n";
        }
    }
}

void DataManager::saveCurrencies() const {
    std::ofstream ratesFile(FILE_RATES);
    if (ratesFile.is_open()) {
        ratesFile.imbue(std::locale::classic());
        for (const auto& curr : m_currencies) {
            curr.serialize(ratesFile);
        }
    }
}

void DataManager::saveTransactions() const {
    std::ofstream txFile(FILE_TRANSACTIONS);
    if (txFile.is_open()) {
        txFile.imbue(std::locale::classic());
        for (const auto& tx : m_transactions) {
            tx->serialize(txFile);
        }
    }
}

void DataManager::saveAllData() {
    saveUsers();
    saveCurrencies();
    saveTransactions();
}

void DataManager::appendTransactionToFile(const Transaction& tx) const {
    std::ofstream file(FILE_TRANSACTIONS, std::ios::app);
    if (file.is_open()) {
        file.imbue(std::locale::classic());
        tx.serialize(file);
    }
}

User *DataManager::authenticate(const std::string &username, const std::string &password) {
    for (auto& user : m_users) {
        if (user.getUsername() == username) {
            if (user.getPassword() == password) {
                return &user;
            }
            return nullptr;
        }
    }
    return nullptr;
}

void DataManager::addUser(const std::string &username, const std::string &password) {
    for (auto& user : m_users) {
        if (user.getUsername() == username) {
            throw std::invalid_argument("User with this username already exists.");
        }
    }
    m_users.emplace_back(username, password, 0);
    saveUsers();
}

bool DataManager::deleteUser(const std::string &username) {
    if (username == "admin") {
        std::cerr << "Cannot delete default admin." << std::endl;
        return false;
    }
    auto it = std::find_if(m_users.begin(), m_users.end(),
        [&username](const User& user) {
        return user.getUsername() == username;
        });
    if (it != m_users.end()) {
        m_users.erase(it);
        saveUsers();
        return true;
    }
    return false;
}

const std::vector<User>& DataManager::getUsers() const {
    return m_users;
}

bool DataManager::changeUserPassword(const std::string &username, const std::string &newPassword) {
    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [&username](const User& u) { return u.getUsername() == username; });

    if (it != m_users.end()) {
        it->setPassword(newPassword);
        saveUsers();
        return true;
    }
    return false;
}


const std::vector<Currency>& DataManager::getCurrencies() const {
    return m_currencies;
}

Currency* DataManager::getCurrencyByCode(const std::string& code) {
    for (auto& currency : m_currencies) {
        if (currency.getCode() == code) {
            return &currency;
        }
    }
    return nullptr;
}

void DataManager::addCurrency(const std::string& code, double buy, double sell) {
    if (getCurrencyByCode(code) != nullptr) {
        throw std::invalid_argument("Currency already exists.");
    }
    m_currencies.emplace_back(code, buy, sell);
    saveCurrencies();
}

void DataManager::updateCurrencyRate(const std::string& code, double buy, double sell) {
    Currency* curr = getCurrencyByCode(code);
    if (curr) {
        curr->setRates(buy, sell);
        saveCurrencies();
    } else {
        throw std::runtime_error("Currency not found!");
    }
}

bool DataManager::deleteCurrency(const std::string& code) {
    auto it = std::find_if(m_currencies.begin(), m_currencies.end(),
                           [&code](const Currency& c) { return c.getCode() == code; });

    if (it != m_currencies.end()) {
        m_currencies.erase(it);
        saveCurrencies();
        return true;
    }
    return false;
}






