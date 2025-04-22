#include <ncurses.h>
#include "aquamatic.h"

const char *menu_options[NUM_OPTIONS] = {
    "Temperature",
    "pH",
    "kH (coming soon)",
    "gH (coming soon)",
    "Salinity",
    "Nitrates/Nitrites/Ammonia (coming soon)",
    "Calcium/Magnesium (coming soon)",
    "Presets",
    "Update"
};


static void doublecheck_kill_daemon(void);
static void show_sensor_menu(const char *Title);

void run_tui(void)
{
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
    keypad(stdscr, TRUE); // Enable arrow key input

    int highlight = 0;
    int choice = -1;
    int ch;

    while (1) {
        clear();
        mvprintw(2, 2, "Aquamatic Control Panel");
        mvprintw(3, 2, "Use arrow keys to navigate, ENTER to select, 'q' to quit.");

        // Print menu options
        for (int i = 0; i < NUM_OPTIONS; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(5 + i, 4, "%s", menu_options[i]);
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }

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
            case 'q': //fallthrough
            case 'Q':
                endwin();
                return;
        }

        // Handle menu selection
        if (choice != -1) {
            clear();
            switch (choice) {
                // case -2:
                //     run_daemon();
                //     mvprintw(10, 4, "Daemon started.");
                //     break;
                // case -1:
                //     doublecheck_kill_daemon();
                //     // mvprintw(10, 4, "Daemon (PID %d) terminated.", pid);
                //     break;
                case 0:
                    show_sensor_menu("Temperature");
                    // mvprintw(10, 4, "Temperature: %.2f°C", get_temperature());
                    break;
                case 1:
                    show_sensor_menu("pH");
                    break;
                case 2:
                    // show_sensor_menu("kH");
                    // mvprintw(10, 4, "Salinity: %.2f PSU", get_salinity());
                    break;
                case 3:
                    // show_sensor_menu("gH");
                    break;
                case 4:
                    show_sensor_menu("Salinity");
                    // show_sensor_menu("Salinity");
                    break;
                case 5:
                    // show_sensor_menu("Ammonia/Nitrites/Nitrates");
                    break;
                case 6:
                    // show_sensor_menu("Calcium/Magnesium");
                    break;
                case 7:
                    show_preset_menu();
                    break;
                case 8:
                    if (kill(pid, SIGUSR1) != 0) {
                        mvprintw(10, 4, "Failed to send signal to daemon");
                    } else {
                        mvprintw(10, 4, "Signal sent to daemon");
                    }
                    // signal(SIGUSR1, tui_signal_handler);
                    break;
            }
            // mvprintw(12, 4, "Press any key to return...");
            refresh();
            // getch(); // Wait for user input before returning
            choice = -1;
        }
    }

    endwin();  // End ncurses
    printf("Sending User Input to Daemon");
}


static void doublecheck_kill_daemon(void)
{
    
    clear();
    mvprintw(10, 4, "Are you sure you want to kill the daemon? (y/n)");
    refresh();

    int ch = getch();
    if (ch == 'y' || ch == 'Y') {
        // mvprintw(12, 4, "");
        quit_daemon();
        mvprintw(12, 4, "Daemon terminated.");
    } else {
        mvprintw(12, 4, "Operation canceled.");
    }

    refresh();
    getch();
}

static void show_sensor_menu(const char *title)
{
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
            plot_t plot_func = get_plot_function(title);
            switch (choice) {
                case 0:
                    werase(menu_win);
                    box(menu_win, 0, 0);

                    char current_temperature[512];
                    read_temperature_from_file(current_temperature);

                    char buffer[64];
                    snprintf(buffer, sizeof(buffer), "Current Temp: %s", current_temperature);

                    mvwprintw(menu_win, height / 2, (width - strlen(buffer)) / 2, "%s", buffer);
                    // mvwprintw(menu_win, height - 2, (width - 28) / 2, "Press any key to return");
                    wrefresh(menu_win);

                    getch();
                    break;
                case 1:
                    message = "Threshold setting coming soon!";
                    break;
                case 2: 
                    delwin(menu_win);
                    endwin();
                    return;
            }

            mvwprintw(menu_win, height / 2, (width - strlen(message)) / 2, "%s", message);
            wrefresh(menu_win);
            getch();
            choice = -1;
        }
    }

    delwin(menu_win);
    endwin();
}
