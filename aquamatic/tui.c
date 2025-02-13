#include <signal.h>
#include <stdio.h>
#include <ncurses.h>

#include "aquamatic.h"
#include "preset.h"

#define NUM_OPTIONS 11

const char *menu_options[NUM_OPTIONS] = {
    "Start Daemon",
    "Kill Daemon",
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


void doublecheck_kill_daemon();

void show_sensor_menu(const char *Title); // , float (*set_value)());

void run_tui() {
    FILE *pid_file = fopen(PID_FILE, "r");
    pid_t pid;

    if (!pid_file) {
        printf("No running daemon found.\n");
    //     // printf("Would you like to start aquamatic? (y/n)");
    //     // int start_session = getchar();
    //     // if (start_session == 'y') {
    //     //     printf("Starting Daemon");
    //     //     run_daemon();
    //     // } else if (start_session == 'n')
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
                case 0:
                    run_daemon();
                    mvprintw(10, 4, "Daemon started.");
                    break;
                case 1:
                    doublecheck_kill_daemon();
                    // mvprintw(10, 4, "Daemon (PID %d) terminated.", pid);
                    break;
                case 2:
                    show_sensor_menu("Temperature");
                    // mvprintw(10, 4, "Temperature: %.2f°C", get_temperature());
                    break;
                case 3:
                    show_sensor_menu("pH");
                    // mvprintw(10, 4, "pH Level: %.2f", get_ph());
                    break;
                case 4:
                    // show_sensor_menu("kH");
                    // mvprintw(10, 4, "Salinity: %.2f PSU", get_salinity());
                    break;
                case 5:
                    // show_sensor_menu("gH");
                    break;
                case 6:
                    show_sensor_menu("Salinity");
                    // show_sensor_menu("Salinity");
                    break;
                case 7:
                    // show_sensor_menu("Ammonia/Nitrites/Nitrates");
                    break;
                case 8:
                    // show_sensor_menu("Calcium/Magnesium");
                    break;
                case 9:
                    show_preset_menu();
                    break;
                case 10:
                    if (kill(pid, SIGUSR1) != 0) {
                        mvprintw(10, 4, "Failed to send signal to daemon");
                    } else {
                        mvprintw(10, 4, "Signal sent to daemon");
                    }
                    // signal(SIGUSR1, tui_signal_handler);
                    break;
            }
            mvprintw(12, 4, "Press any key to return...");
            refresh();
            getch(); // Wait for user input before returning
            choice = -1;
        }
    }

    endwin();  // End ncurses
    printf("Sending User Input to Daemon");
}


void doublecheck_kill_daemon() {
    
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

void show_sensor_menu(const char *title) {//, float (*set_value)()) {
    int highlight = 0;
    int choice = -1;
    int ch;
    const char *options[] = {
        "View Current Value",
        "Set Temperature",
        "Back"
    };
    const int num_options = 3;

    while (1) {
        clear();
        mvprintw(2, 2, "%s Menu", title);
        mvprintw(3, 2, "Use arrow keys to navigate, ENTER to select.");

        for (int i = 0; i < num_options; i++) {
            if (i == highlight) attron(A_REVERSE);
            mvprintw(5 + i, 4, "%s", options[i]);
            if (i == highlight) attroff(A_REVERSE);
        }

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
                return;
        }

        if (choice != -1) {
            clear();
           switch (choice) {
                case 0: // View Current Value
                    // mvprintw(10, 4, "Current %s: %.2f", title, set_value());
                    break;
                case 1: // Set Threshold (Placeholder)
                    mvprintw(10, 4, "Threshold setting feature coming soon!");
                    break;
                case 2: // Back
                    // refresh();
                    // getch();
                    // choice = -1;
                    return;
            }
            mvprintw(12, 4, "Press any key to return...");
            refresh();
            getch();
            choice = -1;
        }
    }
}


