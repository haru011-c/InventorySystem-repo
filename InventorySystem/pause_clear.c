#include <stdio.h>
#include <stdlib.h>

void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseAndClear(void) {
    printf("\n  Press ENTER to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);   // flush leftover input
    getchar();                                     // wait for ENTER
    clearScreen();
}

int main(void) {
    int choice;
    while (1) {
        printf("1. View Catalog\n2. Exit\nEnter choice: ");
        scanf("%d", &choice);
        if (choice == 2) break;
        printf("\n(catalog output here)\n");
        pauseAndClear();
    }
    return 0;
}
