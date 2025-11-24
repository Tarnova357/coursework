//
// Created by SergiyT on 23.11.2025.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include "DataManager.h"


class Application {
private:
    DataManager dataManager;
    User* currentUser;


    void clearScreen();
    void pause();


    int getIntInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    std::string getStringInput(const std::string& prompt);

    void showLoginMenu();
    void showMainMenu();
    void showHelp();

    void menuAdminManageUsers();
    void menuManageCurrencies();
    void menuTransactions();
    void menuReports();

    void createTransaction(const std::string& type);
    void printTransactionList(const std::vector<const Transaction*>& list);

public:
    Application();
    void run();
};



#endif //APPLICATION_H
