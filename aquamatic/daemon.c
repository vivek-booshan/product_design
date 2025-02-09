// #include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <errno.h>

#include "aquamatic.h"

void ensure_directory_exists(const char *path) {
    // Create the directory if it doesn't exist
    if (mkdir(path, 0755) < 0 && errno != EEXIST) {
        perror("Failed to create PID directory");
        exit(EXIT_FAILURE);
    }
}

void read_pid_file(FILE *pid_file, pid_t *pid) {
        if (fscanf(pid_file, "%d", pid) != 1) {
        printf("Error reading PID file.\n");
        fclose(pid_file);
        return;
    }
}
// Daemon logic to run in the background
void run_daemon() {
    ensure_directory_exists(AQUA_DIR);

    int pid_fd = open(PID_FILE, O_CREAT | O_RDWR, 0666);
    if (pid_fd < 0) {
        perror("Failed to open PID file");
        exit(EXIT_FAILURE);
    }

    // Try to lock the file
    if (flock(pid_fd, LOCK_EX | LOCK_NB) != 0) {
        printf("Daemon is already running.\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS); // Parent exits, daemon continues

    setsid();  // Create a new session

    pid = fork(); // guarantee independence of daemon --> block accidental terminal control
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS); // First child exits, daemon continues

    // Save daemon PID to file
    dprintf(pid_fd, "%d\n", getpid());

    umask(0);
    chdir("/");  // Change working directory to root

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Daemon: periodically check for updates
    while (1) {
        if (tui_flag)  {
            printf("Daemon successfully received user input from TUI\n");
            FILE *file = fopen(STATE_FILE, "r");
            if (file) {
                int state;
                if (fscanf(file, "%d", &state) == 1) {
                    // Daemon uses the state value
                    printf("Daemon running with state: %d\n", state);
                }
                fclose(file);
            } else {
                printf("No state file found, using default state.\n");
            }

            tui_flag = 0; // reset flag
        }

        sleep(1);  // Check every second
    }
}

void quit_daemon() {
    FILE *pid_file = fopen(PID_FILE, "r");
    if (!pid_file) {
        printf("No running daemon found.\n");
        return;
    }

    pid_t pid;
    read_pid_file(pid_file, &pid);
    fclose(pid_file);


    if (kill(pid, SIGTERM) != 0) {
        if (errno == ESRCH) {
            // ESRCH: No such process
            printf("Process with PID %d not found.\n", pid);
        } else if (errno == EPERM) {
            // EPERM: Operation not permitted (e.g., lack of permission)
            printf("Permission denied: Unable to terminate process with PID %d.\n", pid);
        } else {
            // Other error
            perror("Failed to terminate daemon");
        }
    } else {
        printf("Daemon (PID %d) terminated.\n", pid);
        unlink(PID_FILE);  // Remove PID file
    }
}
