#include <iostream>
#include <limits>
#include <cstdlib>

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseAndClear() {
    std::cout << "\n  Press ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    clearScreen();
}

int main() {
    int choice;
    while (true) {
        std::cout << "1. View Catalog\n2. Exit\nEnter choice: ";
        std::cin >> choice;
        if (choice == 2) break;
        std::cout << "\n(catalog output here)\n";
        pauseAndClear();
    }
    return 0;
}
