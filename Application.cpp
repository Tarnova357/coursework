//
// Created by SergiyT on 23.11.2025.
//

#include "Application.h"

#include <limits>

#include "BuyTransaction.h"
#include "SellTransaction.h"


Application::Application() : currentUser(nullptr) {}

void Application::run() {
    while (true) {
        if (currentUser == nullptr) {
            showLoginMenu();
        } else {
            showMainMenu();
        }
    }
}

void Application::clearScreen() {
    system("cls");
}

void Application::pause() {
    std::cout << "\nНатисніть Enter щоб продовжити...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int Application::getIntInput() {
    int value;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Невірний ввід. Будь ласка, введіть ціле число: ";
    }
    return value;
}

double Application::getDoubleInput() {
    double value;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Невірний ввід. Введіть число (напр. 100.50): ";
    }
    return value;
}

std::string Application::getStringInput(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::cin >> value;
    return value;

}

void Application::showLoginMenu() {
    clearScreen();
    std::cout << "=== ПУНКТ ОБМІНУ ВАЛЮТ ===\n";
    std::cout << "1. Вхід\n";
    std::cout << "0. Вихід\n";
    std::cout << "> ";

    int choice = getIntInput();

    switch (choice) {
        case 0:
            std::cout << "До побачення!\n";
        exit(0);
        case 1: {
            std::string username = getStringInput("Логін: ");
            std::string password = getStringInput("Пароль: ");

            User* user = dataManager.authenticate(username, password);
            if (user) {
                currentUser = user;
                std::cout << "Вхід успішний.\n";
            } else {
                std::cout << "Помилка: Невірний логін або пароль.\n";
                pause();
            }
            break;
        }
        default:
            std::cout << "Невірний вибір.\n";
        pause();
    }
}

void Application::showMainMenu() {
    clearScreen();
    std::cout << "=== ГОЛОВНЕ МЕНЮ (" << currentUser->getUsername() << ") ===\n";
    std::cout << "1. Транзакції (Нова / Редагувати / Видалити)\n";
    std::cout << "2. Курси валют\n";

    if (currentUser->isAdmin()) {
        std::cout << "3. [АДМІН] Керування користувачами\n";
    }

    std::cout << "4. Звіти та Пошук\n";
    std::cout << "5. Змінити свій пароль\n";
    std::cout << "6. Допомога\n";
    std::cout << "0. Вихід з акаунту\n";
    std::cout << "> ";

    int choice = getIntInput();

    try {
        switch (choice) {
            case 1: menuTransactions(); break;
            case 2: menuManageCurrencies(); break;
            case 3:
                if (currentUser->isAdmin()) menuAdminManageUsers();
                else { std::cout << "Доступ заборонено.\n"; pause(); }
            break;
            case 4: menuReports(); break;
            case 5: {
                std::string newP = getStringInput("Введіть новий пароль: ");
                if (dataManager.changeUserPassword(currentUser->getUsername(), newP))
                    std::cout << "Успішно: Пароль змінено.\n";
                else
                    std::cout << "Помилка: Користувача не знайдено.\n";
                pause();
                break;
            }
            case 6: showHelp(); break;
            case 0: currentUser = nullptr; break;
            default: std::cout << "Невірний вибір.\n"; pause();
        }
    } catch (const std::exception& e) {
        std::cout << "КРИТИЧНА ПОМИЛКА в меню: " << e.what() << "\n";
        pause();
    }
}

void Application::showHelp() {
    clearScreen();
    std::cout << "=== ДОПОМОГА ===\n";
    std::cout << "- Логін адміністратора за замовчуванням: 'admin' (пароль: 'admin123')\n";
    std::cout << "- Транзакції можна редагувати за ID (тільки дані клієнта).\n";
    std::cout << "- Звіти дозволяють фільтрувати дані (наприклад, 'Клієнти, що купують USD').\n";
    pause();
}

void Application::menuAdminManageUsers() {
    clearScreen();
    std::cout << "=== КОРИСТУВАЧІ ===\n";
    for (const auto& u : dataManager.getUsers()) {
        std::cout << " - " << u.getUsername() << (u.isAdmin() ? " [АДМІН]" : "") << "\n";
    }
    std::cout << "\n1. Додати користувача\n2. Видалити користувача\n3. Скинути пароль користувача\n0. Назад\n> ";

    int c = getIntInput();
    if (c == 0) return;

    try {
        switch (c) {
            case 1: {
                std::string myCurrentLogin = currentUser->getUsername();
                std::string u = getStringInput("Новий логін: ");
                std::string p = getStringInput("Новий пароль: ");
                dataManager.addUser(u, p);
                currentUser = dataManager.getUserByName(myCurrentLogin);
                std::cout << "Успішно: Користувача додано.\n";
                break;
            }
            case 2: {
                if (dataManager.deleteUser(getStringInput("Логін для видалення: ")))
                    std::cout << "Успішно: Видалено.\n";
                else
                    std::cout << "Помилка: Не знайдено або не можна видалити Адміна.\n";
                break;
            }
            case 3: {
                std::string u = getStringInput("Логін: ");
                std::string p = getStringInput("Новий пароль: ");
                if (dataManager.changeUserPassword(u, p))
                    std::cout << "Успішно: Пароль скинуто.\n";
                else
                    std::cout << "Помилка: Не знайдено.\n";
                break;
            }
            default:
                std::cout << "Невірний вибір.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "ПОМИЛКА: " << e.what() << "\n";
    }
    pause();
}

void Application::menuManageCurrencies() {
    clearScreen();
    std::cout << "=== КУРСИ ВАЛЮТ ===\n";
    for (const auto& c : dataManager.getCurrencies()) c.display();
    std::cout << "\n1. Оновити курс\n2. Додати валюту\n3. Видалити валюту\n0. Назад\n> ";

    int c = getIntInput();
    if (c == 0) return;

    try {
        switch (c) {
            case 1: {
                std::string code = getStringInput("Код валюти (напр. USD): ");
                std::cout << "Введіть ціну покупки: ";
                double buy = getDoubleInput();
                std::cout << "\n" << "Введіть ціну продажу:" << "\n";
                double sell = getDoubleInput();
                dataManager.updateCurrencyRate(code, buy, sell);
                std::cout << "Успішно: Курс оновлено.\n";
                break;
            }
            case 2: {
                std::string code = getStringInput("Новий код валюти: ");
                double buy = getDoubleInput();
                double sell = getDoubleInput();
                dataManager.addCurrency(code, buy, sell);
                std::cout << "Успішно: Валюту додано.\n";
                break;
            }
            case 3: {
                std::string code = getStringInput("Код для видалення: ");
                if (dataManager.deleteCurrency(code))
                    std::cout << "Успішно: Видалено.\n";
                else
                    std::cout << "Помилка: Не знайдено.\n";
                break;
            }
            default:
                std::cout << "Невірний вибір.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "ПОМИЛКА: " << e.what() << "\n";
    }
    pause();
}

void Application::menuTransactions() {
    while (true) {
        clearScreen();
        std::cout << "=== ТРАНЗАКЦІЇ ===\n";
        std::cout << "1. Продати валюту (Клієнт купує)\n";
        std::cout << "2. Купити валюту (Клієнт продає)\n";
        std::cout << "3. Редагувати транзакцію (Дані клієнта)\n";
        std::cout << "4. Видалити транзакцію\n";
        std::cout << "0. Назад\n> ";

        int c = getIntInput();
        if (c == 0) break;

        // [ЗМІНА] Switch case
        switch (c) {
            case 1: createTransaction("ПРОДАЖ"); break;
            case 2: createTransaction("КУПІВЛЯ"); break;

            case 3:
            case 4: {
                std::cout << "Введіть ID транзакції: ";
                int id = getIntInput();
                try {
                    if (c == 3) { // Edit
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Нове ім'я клієнта: ";
                        std::string n;
                        std::getline(std::cin, n);

                        std::string s = getStringInput("Нова серія: ");
                        std::string num = getStringInput("Новий номер: ");

                        if (dataManager.editTransactionClient(id, n, s, num))
                            std::cout << "Успішно: Оновлено.\n";
                        else
                            std::cout << "Помилка: ID не знайдено.\n";
                    } else { // Delete
                        if (dataManager.deleteTransaction(id))
                            std::cout << "Успішно: Видалено.\n";
                        else
                            std::cout << "Помилка: ID не знайдено.\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "ПОМИЛКА: " << e.what() << "\n";
                }
                pause();
                break;
            }
            default:
                std::cout << "Невірний вибір.\n";
                pause();
        }
    }
}

void Application::createTransaction(const std::string& type) {
    try {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n--- Нова операція: " << type << " ---\n";

        std::cout << "ПІБ Клієнта: ";
        std::string name;
        std::getline(std::cin, name);

        std::string series = getStringInput("Серія паспорта: ");
        std::string number = getStringInput("Номер паспорта: ");
        std::string code = getStringInput("Код валюти: ");
        std::cout << "Сума у валюті: ";
        double amount = getDoubleInput();

        Currency* curr = dataManager.getCurrencyByCode(code);
        if (!curr) throw std::runtime_error("Валюту не знайдено.");

        std::unique_ptr<Transaction> tx;
        if (type == "КУПІВЛЯ") {
            tx = std::make_unique<BuyTransaction>(Client(name, series, number), code, amount);
        } else {
            tx = std::make_unique<SellTransaction>(Client(name, series, number), code, amount);
        }

        tx->calculate(curr->getBuyRate(), curr->getSellRate());

        std::cout << "\nПопередній перегляд:\n";
        tx->display();

        if (getStringInput("Підтвердити? (y/n): ") == "y") {
            dataManager.addTransaction(std::move(tx));
            std::cout << "Успішно: Транзакцію збережено.\n";
        } else {
            std::cout << "Скасовано.\n";
        }

    } catch (const std::exception& e) {
        std::cout << "ТРАНЗАКЦІЯ НЕВДАЛА: " << e.what() << "\n";
    }
    pause();
}

void Application::menuReports() {
    while (true) {
        clearScreen();
        std::cout << "=== ЗВІТИ ТА ПОШУК ===\n";
        std::cout << "1. Всі (Сорт. за датою)\n";
        std::cout << "2. Всі (Сорт. за сумою)\n";
        std::cout << "3. Пошук за ім'ям клієнта\n";
        std::cout << "4. Фільтр за валютою\n";
        std::cout << "5. Фільтр за типом (КУПІВЛЯ/ПРОДАЖ)\n";
        std::cout << "6. Фільтр за мін. сумою\n";
        std::cout << "7. [Приклад] Клієнти, що купують/продають певну валюту\n";
        std::cout << "0. Назад\n> ";

        int c = getIntInput();
        if (c == 0) break;

        std::vector<const Transaction*> list;

        try {
            // [ЗМІНА] Switch case
            switch (c) {
                case 1:
                    dataManager.sortTransactionsByDate(false);
                    for (const auto& t : dataManager.getTransactions()) list.push_back(t.get());
                    break;
                case 2:
                    dataManager.sortTransactionsByAmount(false);
                    for (const auto& t : dataManager.getTransactions()) list.push_back(t.get());
                    break;
                case 3: {
                    std::cin.ignore(1000, '\n');
                    std::cout << "Частина імені: ";
                    std::string n; std::getline(std::cin, n);
                    list = dataManager.searchTransactionsByClient(n);
                    break;
                }
                case 4:
                    list = dataManager.filterTransactionsByCurrency(getStringInput("Код: "));
                    break;
                case 5:
                    list = dataManager.filterTransactionsByType(getStringInput("Тип (КУПІВЛЯ/ПРОДАЖ): "));
                    break;
                case 6:
                    list = dataManager.filterTransactionsByMinAmount(getDoubleInput());
                    break;
                case 7: {
                    std::string code = getStringInput("Валюта (напр. USD): ");
                    std::cout << "Що робить клієнт? (1-Купує, 2-Продає): ";
                    std::string type = (getIntInput() == 1) ? "ПРОДАЖ" : "КУПІВЛЯ";
                    list = dataManager.filterTransactionsByCurrencyAndType(code, type);
                    break;
                }
                default:
                    std::cout << "Невірний вибір.\n";
            }

            // Виводимо список, якщо вибір був валідним
            if (c >= 1 && c <= 7) {
                printTransactionList(list);
            }

        } catch (const std::exception& e) {
            std::cout << "Помилка пошуку: " << e.what() << "\n";
        }
        pause();
    }
}

void Application::printTransactionList(const std::vector<const Transaction*>& list) {
    std::cout << "\n--- РЕЗУЛЬТАТИ ---\n";
    if (list.empty()) {
        std::cout << "Нічого не знайдено.\n";
    } else {
        for (const auto* t : list) {
            t->display();
        }
        std::cout << "Всього: " << list.size() << " записів.\n";
    }
}