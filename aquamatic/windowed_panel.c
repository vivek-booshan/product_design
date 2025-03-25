
#include <ncurses.h>
#include <stdio.h>
// #include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "aquamatic.h"
#include "preset.h"
#define WIDTH 40
#define HEIGHT 15

void show_sensor_menu(const char *Title);

void run_tui(void) {
    FILE *pid_file = fopen(PID_FILE, "r");
    pid_t pid;

    if (!pid_file) {
        printf("No running daemon found.\n");
        return;
    }

    read_pid_file(pid_file, &pid);
    fclose(pid_file);

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width); // Get terminal size

    int start_y = (term_height - HEIGHT) / 2;
    int start_x = (term_width - WIDTH) / 2;

    WINDOW *menu_win = newwin(HEIGHT, WIDTH, start_y, start_x);
    box(menu_win, 0, 0);  // Draw border
    refresh();
    wrefresh(menu_win);

    int highlight = 0;
    int choice = -1;
    int ch;

    while (1) {
        werase(menu_win);
        box(menu_win, 0, 0); // Redraw border

        mvwprintw(menu_win, 1, (WIDTH - 24) / 2, "Aquamatic Control Panel");
        mvwprintw(menu_win, 2, 2, "Use ↑/↓ to navigate, ENTER to select, 'q' to quit.");

        // Print menu options inside window
        for (int i = 0; i < NUM_OPTIONS; i++) {
            if (i == highlight) {
                wattron(menu_win, A_REVERSE);
            }
            mvwprintw(menu_win, 4 + i, 4, "%s", menu_options[i]);
            if (i == highlight) {
                wattroff(menu_win, A_REVERSE);
            }
        }

        wrefresh(menu_win);
        ch = getch();

        switch (ch) {
            case KEY_UP:
                highlight = (highlight == 0) ? NUM_OPTIONS - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == NUM_OPTIONS - 1) ? 0 : highlight + 1;
                break;
            case 10: // Enter key
                choice = highlight;
                break;
            case 'q': // Fallthrough
            case 'Q':
                delwin(menu_win);
                endwin();
                return;
        }

        // Handle menu selection
        if (choice != -1) {
            werase(menu_win);
            box(menu_win, 0, 0);

            switch (choice) {
                case 0:
                    show_sensor_menu("Temperature");
                    break;
                case 1:
                    show_sensor_menu("pH");
                    break;
                case 4:
                    show_sensor_menu("Salinity");
                    break;
                case 7:
                    show_preset_menu();
                    break;
                case 8:
                    if (kill(pid, SIGUSR1) != 0) {
                        mvwprintw(menu_win, HEIGHT - 3, 4, "Failed to send signal to daemon");
                    } else {
                        mvwprintw(menu_win, HEIGHT - 3, 4, "Signal sent to daemon");
                    }
                    break;
            }

            mvwprintw(menu_win, HEIGHT - 2, 4, "Press any key to return...");
            wrefresh(menu_win);
            getch();
            choice = -1;
        }
    }

    delwin(menu_win);
    endwin();
    printf("Sending User Input to Daemon");
}

void show_sensor_menu(const char *title) {
    int highlight = 0;
    int choice = -1;
    int ch;
    const char *options[] = {
        "View Current Value",
        "Set target",
        "Back"
    };
    const int num_options = 3;

    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);  // Hide cursor
    keypad(stdscr, TRUE); // Enable arrow keys

    // Define window size
    int height = 10, width = 40;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    // Create window
    WINDOW *menu_win = newwin(height, width, start_y, start_x);
    box(menu_win, 0, 0); // Draw border

    refresh();

    while (1) {
        werase(menu_win);
        box(menu_win, 0, 0); // Redraw the border

        // Center title correctly inside [ ]
        int title_length = strlen(title); // + 4;  // Account for "[ ]"
        int title_x = (width - title_length) / 2;
        mvwprintw(menu_win, 1, title_x, "%s", title);

        // Instructions for user
        // mvwprintw(menu_win, 2, 2, "Use up/down arrows to navigate, ENTER to select.");

        // Print menu options, properly centered
        for (int i = 0; i < num_options; i++) {
            int option_x = (width - strlen(options[i])) / 2; // Center horizontally
            if (i == highlight) wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, 4 + i, option_x, "%s", options[i]);
            if (i == highlight) wattroff(menu_win, A_REVERSE);
        }

        wrefresh(menu_win);
        ch = getch();

        switch (ch) {
            case KEY_UP:
                highlight = (highlight == 0) ? num_options - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == num_options - 1) ? 0 : highlight + 1;
                break;
            case 10:  // Enter key
                choice = highlight;
                break;
            case 'q':
                delwin(menu_win);
                endwin();
                return;
        }

        if (choice != -1) {
            werase(menu_win);
            box(menu_win, 0, 0);

            // Display action result, centered
            const char *message;
            switch (choice) {
                case 0: message = "Viewing current value..."; break;
                case 1: message = "Threshold setting coming soon!"; break;
                case 2: 
                    delwin(menu_win);
                    endwin();
                    return;
            }

            mvwprintw(menu_win, height / 2, (width - strlen(message)) / 2, "%s", message);
            mvwprintw(menu_win, height - 2, (width - 24) / 2, "Press any key to return...");
            wrefresh(menu_win);
            getch();
            choice = -1;
        }
    }

    delwin(menu_win);
    endwin();
}
