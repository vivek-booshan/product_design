#include "aquamatic.h"

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
