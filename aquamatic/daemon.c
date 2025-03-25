#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>

#include "aquamatic.h"


void ensure_directory_exists(const char *path);
void read_pid_file(FILE *pid_file, pid_t *pid); 
void get_pid(pid_t *pid);

void run_daemon(void) {
    signal(SIGUSR1, tui_signal_handler);

    ensure_directory_exists(AQUA_DIR);

    int pid_fd = open(PID_FILE, O_CREAT | O_RDWR, 0666);
    if (pid_fd < 0) {
        perror("Failed to open PID file");
        exit(EXIT_FAILURE);
    }

    int state_fd = open(STATE_FILE, O_CREAT | O_RDWR, 0666);
    if (state_fd < 0) {
        perror("Failed to open STATE file");
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
    int i = 0;
    while (1) {
        if (tui_flag)  {
            // printf("Daemon successfully received user input from TUI\n");
            FILE *file = fopen(STATE_FILE, "a");
            if (file) {
                // int state;
                // if (fscanf(file, "%d", &state) == 1) {
                    // Daemon uses the state value
                char buffer[20];
                get_timestamp(buffer, sizeof(buffer));
                fprintf(file, "[%s] Daemon received update", buffer);
                    // printf("Daemon running with state: %d\n", state);
            }
            fclose(file);
            printf("%d", tui_flag);
            tui_flag = 0; // reset flag
        }

        // fprintf();
        sleep(1);  // Check every second
    }
}


void quit_daemon(void) {
    pid_t pid;
    get_pid(&pid);

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
        printf("\nDaemon (PID %d) terminated.\n", pid);
        unlink(PID_FILE);  // Remove PID file
    }
}

void ensure_directory_exists(const char *path) {
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

void get_pid(pid_t *pid) {
    FILE *file = fopen(PID_FILE, "r");
    if (!file) {
        printf("No running daemon found.\n");
        return;
    }
    read_pid_file(file, pid);
    fclose(file);
}

