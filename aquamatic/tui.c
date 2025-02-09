#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "aquamatic.h"

void run_tui() {
    FILE *pid_file = fopen(PID_FILE, "r");
    pid_t pid;

    if (!pid_file) {
        printf("No running daemon found.\n");
        // printf("Would you like to start aquamatic? (y/n)");
        // int start_session = getchar();
        // if (start_session == 'y') {
        //     printf("Starting Daemon");
        //     run_daemon();
        // } else if (start_session == 'n')
        return;
    }
    read_pid_file(pid_file, &pid);
    fclose(pid_file);

    initscr();
    noecho();
    curs_set(FALSE);

    int ch;
    int new_state = 1;

    while (1) {
        clear();
        mvprintw(2, 2, "Aquamatic daemon is running...");
        mvprintw(4, 2, "Current state: %d", new_state);
        mvprintw(6, 2, "Enter a new state and press ENTER to update:");
        mvprintw(8, 2, "Press 'q' to quit the TUI.");
        refresh();

        ch = getch();
        if (ch == 'q' || ch == 'Q') {
            kill(pid, SIGUSR1);
            break;  // Exit TUI if 'q' is pressed
        }

        if (ch == 10) {  // Enter key pressed
            char input[10];
            getstr(input);
            new_state = atoi(input);  // Convert input to integer

            // Update the state file to notify the daemon
            FILE *file = fopen(STATE_FILE, "w");
            if (file) {
                fprintf(file, "%d", new_state);
                fclose(file);
            }
        }
    }

    endwin();  // End ncurses
    printf("Sending User Input to Daemon");
}

