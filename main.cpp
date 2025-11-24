#include <iostream>
#include <windows.h>
#include "Application.h"

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::setlocale(LC_ALL, ".UTF8");
     try {
         Application app;
         app.run();
     } catch (const std::exception& e) {
         std::cerr << "ÊÐÈÒÈ×ÍÀ ÏÎÌÈËÊÀ: " << e.what() << std::endl;
         return 1;
     } catch (...) {
         std::cerr << "ÊÐÈÒÈ×ÍÀ ÍÅÂ²ÄÎÌÀ ÏÎÌÈËÊÀ!" << std::endl;
         return 1;
     }
    return 0;
}
