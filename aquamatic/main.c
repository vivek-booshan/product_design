#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aquamatic.h"

volatile int tui_flag = 0;

void tui_signal_handler(int signum) {
    tui_flag = 1;
}

void get_timestamp(char *buffer, size_t buffer_size) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);               // Get current time
    timeinfo = localtime(&rawtime); // Convert to local time

    // Format the time as "YY-MM-DD HH:MM:SS"
    strftime(buffer, buffer_size, "%y-%m-%d %H:%M:%S", timeinfo);
}


void print_help(const char *program_name) {
    printf("Usage: %s <command>\n", program_name);
    printf("\nCommands:\n");
    printf("  start   - Starts the Aquamatic daemon.\n");
    printf("  kill    - Stops the running Aquamatic daemon.\n");
    printf("  tui     - Launches the terminal user interface (TUI).\n");
    printf("  help    - Displays this help message.\n");
    printf("\nDescription:\n");
    printf("  Aquamatic is a system for managing and automating aquarium systems.\n");
    printf("  The daemon must be running for TUI mode to work properly.\n");
    printf("  It is not recommended to manually start or kill the daemon.\n");
    printf("\nExamples:\n");
    printf("  %s start   # Start the daemon\n", program_name);
    printf("  %s kill    # Stop the daemon\n", program_name);
    printf("  %s tui     # Open the TUI\n", program_name);
    printf("\n");
}

int main(int argc, char *argv[]) {
    // Ensure the user provided at least one argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <start|kill|tui|help>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Get the command from the argument
    const char *flag = argv[1];

    // Decide which function to call based on the argument
    if (strcmp(flag, "start") == 0) {
        // signal(SIGUSR1, tui_signal_handler);
        run_daemon();
    } else if (strcmp(flag, "kill") == 0) {
        quit_daemon();
    } else if (strcmp(flag, "tui") == 0) {
        run_tui();
    } else if (strcmp(flag, "help") == 0) {
        print_help(argv[0]);
    } else {
        fprintf(stderr, "Invalid flag. Usage: %s <start|kill|tui|help>\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
