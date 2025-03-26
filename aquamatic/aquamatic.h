#ifndef AQUAMATIC_H
#define AQUAMATIC_H

#include <stdint.h>
#include <stdio.h>
#include <sys/file.h>

#define AQUA_DIR "/tmp/aquamatic"
#define PID_FILE "/tmp/aquamatic/daemon.pid"
#define STATE_FILE "/tmp/aquamatic/state.txt"

extern volatile int tui_flag;

void read_pid_file(FILE *pid_file, pid_t *pid);
void run_daemon();
void quit_daemon();
void run_tui();
void tui_signal_handler(int signum);
void get_timestamp(char *buffer, size_t buffer_size);

#endif
