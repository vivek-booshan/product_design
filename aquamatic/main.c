#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "aquamatic.h"

int tui_flag = 0;

void tui_signal_handler(int signum) {
    tui_flag = 1;
}

int main(int argc, char *argv[]) {
    // Ensure the user provided at least one argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <start|kill|tui>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Get the command from the argument
    const char *flag = argv[1];

    // Decide which function to call based on the argument
    if (strcmp(flag, "start") == 0) {
        signal(SIGUSR1, tui_signal_handler);
        run_daemon();
    } else if (strcmp(flag, "kill") == 0) {
        quit_daemon();
    } else if (strcmp(flag, "tui") == 0) {
        run_tui();
    } else {
        fprintf(stderr, "Invalid flag. Usage: %s <start|kill|tui>\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
