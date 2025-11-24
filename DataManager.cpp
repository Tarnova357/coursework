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
                std::cerr <<"[Попередження] Пропущено пошкоджений рядок у users.txt:" <<line << std::endl;
                continue;
            }
            std::string username = line.substr(0, pos);
            std::string password = line.substr(pos + 1);

            try {
                bool isAdmin = (username == "admin");
                m_users.emplace_back(username, password, isAdmin);
            } catch (const std::exception& e) {
                std::cerr << "[Помилка] Не вдалося завантажити користувача: " << e.what() << std::endl;
            }
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
        std::cout << "[Інфо] Адміністратора не знайдено. Створення стандартного адміна..." << std::endl;
        m_users.insert(m_users.begin(), User("admin", "admin123", true));
        saveUsers();
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
                std::cerr << "[Попередження] Пошкоджені дані у файлі курсів. Рядок пропущено: '"
                          << dummy << "'" << std::endl;
                continue;
            }
            m_currencies.emplace_back(temp);
        } catch (const std::exception& e) {
            file.close();
            std::cerr << "[Помилка] Помилка завантаження валюти: " << e.what() << std::endl;
        }
    }
    file.close();
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

        if (!(ss >> id >> type)) {
            std::cerr << "[Попередження] Не вдалося прочитати заголовок транзакції. Рядок пропущено.\n";
            continue;
        }

        if (id > m_lastId) m_lastId = id;
        std::unique_ptr<Transaction> temp;

        try {
            if (type =="КУПІВЛЯ") {
                temp = std::make_unique<BuyTransaction>();
            } else if (type =="ПРОДАЖ") {
                temp = std::make_unique<SellTransaction>();
            } else {
                std::cerr << "[Попередження] Невідомий тип транзакції: '" << type
                          << "' В рядку: '" << line << "'. Пропущено." << std::endl;
                continue;
            }
            temp->setId(id);
            temp->deserialize(ss);
            m_transactions.emplace_back(std::move(temp));
        } catch (const std::exception& e) {
            file.close();
            std::cerr << "[Помилка] Збій завантаження транзакції ID " << id << ": " << e.what() << "\n";
        }
    }
    file.close();
}

void DataManager::saveUsers() const {
    std::ofstream usersFile(FILE_USERS);
    if (usersFile.is_open()) {
        for (const auto& user : m_users) {
            usersFile << user.getUsername() << ":" << user.getPassword() << "\n";
        }
    }
    usersFile.close();
}

void DataManager::saveCurrencies() const {
    std::ofstream ratesFile(FILE_RATES);
    if (ratesFile.is_open()) {
        ratesFile.imbue(std::locale::classic());
        for (const auto& curr : m_currencies) {
            curr.serialize(ratesFile);
        }
    }
    ratesFile.close();
}

void DataManager::saveTransactions() const {
    std::ofstream txFile(FILE_TRANSACTIONS);
    if (txFile.is_open()) {
        txFile.imbue(std::locale::classic());
        for (const auto& tx : m_transactions) {
            tx->serialize(txFile);
        }
    }
    txFile.close();
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
    file.close();
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

User* DataManager::getUserByName(const std::string& username) {
    for (auto& user : m_users) {
        if (user.getUsername() == username) {
            return &user;
        }
    }
    return nullptr;
}

void DataManager::addUser(const std::string &username, const std::string &password) {
    for (auto& user : m_users) {
        if (user.getUsername() == username) {
            throw std::invalid_argument("Користувач з таким логіном вже існує..");
        }
    }
    m_users.emplace_back(username, password, 0);
    saveUsers();
}

bool DataManager::deleteUser(const std::string &username) {
    if (username == "admin") {
        std::cerr << "Неможливо видалити стандартного адміна." << std::endl;
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
        throw std::invalid_argument("Така валюта вже існує.");
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
        throw std::runtime_error("Валюту не знайдено.");
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

void DataManager::addTransaction(std::unique_ptr<Transaction> transaction) {
    if (!transaction) throw std::invalid_argument("Транзакція пуста (NULL).");

    m_lastId++;
    transaction->setId(m_lastId);

    m_transactions.push_back(std::move(transaction));
    appendTransactionToFile(*m_transactions.back());
}

bool DataManager::deleteTransaction(int id) {
    auto it = std::find_if(m_transactions.begin(), m_transactions.end(),
                           [id](const std::unique_ptr<Transaction>& t) { return t->getId() == id; });

    if (it != m_transactions.end()) {
        m_transactions.erase(it);
        saveTransactions();
        return true;
    }
    return false;
}

bool DataManager::editTransactionClient(int id, const std::string& name, const std::string& series, const std::string& num) {
    auto it = std::find_if(m_transactions.begin(), m_transactions.end(),
                           [id](const std::unique_ptr<Transaction>& t) { return t->getId() == id; });

    if (it != m_transactions.end()) {
        (*it)->setClient(Client(name, series, num));
        saveTransactions();
        return true;
    }
    return false;
}

const std::vector<std::unique_ptr<Transaction>>& DataManager::getTransactions() const {
    return m_transactions;
}

std::vector<const Transaction*> DataManager::searchTransactionsByClient(const std::string& partialName) const {
    std::vector<const Transaction*> results;
    for (const auto& tx : m_transactions) {
        if (tx->getClient().getFullName().find(partialName) != std::string::npos) {
            results.push_back(tx.get());
        }
    }
    return results;
}

void DataManager::sortTransactionsByDate(bool ascending) {
    std::sort(m_transactions.begin(), m_transactions.end(),
              [ascending](const std::unique_ptr<Transaction>& a, const std::unique_ptr<Transaction>& b) {
                  if (ascending) return a->getTimestamp() < b->getTimestamp();
                  else return a->getTimestamp() > b->getTimestamp();
              });
}

void DataManager::sortTransactionsByAmount(bool ascending) {
    std::sort(m_transactions.begin(), m_transactions.end(),
              [ascending](const std::unique_ptr<Transaction>& a, const std::unique_ptr<Transaction>& b) {
                  if (ascending) return a->getAmountBase() < b->getAmountBase();
                  else return a->getAmountBase() > b->getAmountBase();
              });
}

std::vector<const Transaction*> DataManager::filterTransactionsByCurrency(const std::string& code) const {
    std::vector<const Transaction*> results;
    for (const auto& tx : m_transactions) {
        if (tx->getCurrencyCode() == code) results.push_back(tx.get());
    }
    return results;
}

std::vector<const Transaction*> DataManager::filterTransactionsByType(const std::string& type) const {
    std::vector<const Transaction*> results;
    for (const auto& tx : m_transactions) {
        if (tx->getOperationType() == type) results.push_back(tx.get());
    }
    return results;
}

std::vector<const Transaction*> DataManager::filterTransactionsByCurrencyAndType(const std::string& code, const std::string& type) const {
    std::vector<const Transaction*> results;
    for (const auto& tx : m_transactions) {
        if (tx->getCurrencyCode() == code && tx->getOperationType() == type) {
            results.push_back(tx.get());
        }
    }
    return results;
}

std::vector<const Transaction*> DataManager::filterTransactionsByMinAmount(double minAmount) const {
    std::vector<const Transaction*> results;
    for (const auto& tx : m_transactions) {
        if (tx->getAmountBase() >= minAmount) results.push_back(tx.get());
    }
    return results;
}


void DataManager::saveReportToFile(const std::vector<const Transaction*>& list, const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Не вдалося створити файл. Перевірте назву або права доступу.");
    }

    file.imbue(std::locale::classic());

    for (const auto* tx : list) {
        tx->serialize(file);
    }
    file.close();
}




