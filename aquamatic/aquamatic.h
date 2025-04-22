#ifndef AQUAMATIC_H
#define AQUAMATIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#ifndef cast
#define cast(Type) (Type)
#endif

#define persistent static

#define AQUA_DIR "/tmp/aquamatic"
#define PID_FILE "/tmp/aquamatic/daemon.pid"
#define STATE_FILE "/tmp/aquamatic/state.txt"
#define TEMP_LOG "/tmp/aquamatic/temperature.txt"
#define PH_LOG "/tmp/aquamatic/pH.txt"

#define SERIAL0 "/dev/serial0"
#define SERIALUSB0 "/dev/ttyUSB0"
#define BAUD_RATE B9600

#define NUM_OPTIONS 9
extern const char *menu_options[NUM_OPTIONS]; // declared in tui.c

typedef int (*plot_t)();

typedef struct setpoint_t {
        float temp;
        float ph;
} setpoint_t;

extern setpoint_t setpoints;

void read_pid_file(FILE *pid_file, pid_t *pid);
void run_daemon(void);
void quit_daemon(void);
void run_tui(void);
void get_timestamp(char *buffer, size_t buffer_size);
void show_preset_menu(void);
void print_help(const char *program_name);
void get_temperature(int serial_port, char *local_buf);
void get_ph(int usb_port, char *local_buf);
void write_data(FILE *writer, char *temperature_buf);
int init_serial_port(int serial_port);
plot_t get_plot_function(const char *title);
void read_temperature_from_file(char *buf);

#endif // AQUAMATIC_H
