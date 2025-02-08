
#include <ncurses.h>

#define MENU_SIZE 4

const char *menu_items[MENU_SIZE] = {
    "Option 1",
    "Option 2",
    "Option 3",
    "Exit"
};

int main() {
    int choice = 0;
    int key;

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Hide cursor

    while (1) {
        // Clear the screen
        clear();

        // Display menu
        for (int i = 0; i < MENU_SIZE; i++) {
            if (i == choice) {
                attron(A_REVERSE); // Highlight selected option
            }
            mvprintw(5 + i, 10, "%s", menu_items[i]);
            attroff(A_REVERSE);
        }

        // Refresh screen
        refresh();

        // Wait for user input
        key = getch();

        switch (key) {
            case KEY_UP:
                if (choice > 0) choice--;
                break;
            case KEY_DOWN:
                if (choice < MENU_SIZE - 1) choice++;
                break;
            case 10: // Enter key
                if (choice == MENU_SIZE - 1) {
                    endwin(); // Exit ncurses mode
                    return 0;
                }
                break;
        }
    }

    return 0;
}
