#include "../include/aquamatic.h"
#include "wiringPi.h"

static inline void ensure_directory_exists(const char *path);
static inline void get_pid(pid_t *pid);

void run_daemon(void)
{
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

    int temperature_port = open(SERIAL0, O_RDWR | O_NOCTTY | O_NDELAY);
    if (init_serial_port(temperature_port) != 0) return;
    FILE *temp_writer = fopen(TEMP_LOG, "a");
    if (!temp_writer) return;

    int ph_port = open(SERIALUSB0, O_RDONLY | O_NOCTTY | O_NDELAY);
    if (init_serial_port(ph_port) != 0) return;
    FILE *ph_writer = fopen(PH_LOG, "a");
    if (!ph_writer) return;

    wiringPiSetup();
    pinMode(HEATER_PIN, OUTPUT);
    pinMode(PHUP_PIN, OUTPUT);
    pinMode(PHDOWN_PIN, OUTPUT);

    while (1) {
        char temp_buf[512], ph_buf[512];
        memset(&temp_buf, '\0', sizeof(temp_buf));
        memset(&ph_buf, '\0', sizeof(ph_buf));

        get_temperature(temperature_port, temp_buf);
        get_ph(ph_port, ph_buf);

        control_heater(atof(temp_buf));
        control_ph(atof(ph_buf));
        // 
        write_data(temp_writer, temp_buf);
        write_data(ph_writer, ph_buf);

        sleep(1); 
    }
}


void quit_daemon(void)
{
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

static inline void ensure_directory_exists(const char *path)
{
    if (mkdir(path, 0755) < 0 && errno != EEXIST) {
        perror("Failed to create PID directory");
        exit(EXIT_FAILURE);
    }
}

inline void read_pid_file(FILE *pid_file, pid_t *pid)
{
    if (fscanf(pid_file, "%d", pid) != 1) {
        printf("Error reading PID file.\n");
        fclose(pid_file);
    }
}

static inline void get_pid(pid_t *pid)
{
    FILE *file = fopen(PID_FILE, "r");
    if (!file) {
        printf("No running daemon found.\n");
        return;
    }
    read_pid_file(file, pid);
    fclose(file);
}

inline void get_timestamp(char *buffer, size_t buffer_size)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime); 

    strftime(buffer, buffer_size, "%y-%m-%d %H:%M:%S", timeinfo);
}
