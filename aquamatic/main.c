#include <string.h>
#include "aquamatic.h"

volatile int tui_flag = 0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <start|kill|tui|help>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *flag = argv[1];
    if (strcmp(flag, "start") == 0) {
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
