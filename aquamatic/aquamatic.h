#ifndef AQUAMATIC_H
#define AQUAMATIC_H

#include <stdint.h>
#include <stdio.h>
#include <sys/file.h>

#define AQUA_DIR "/tmp/aquamatic"
#define PID_FILE "/tmp/aquamatic/daemon.pid"
#define STATE_FILE "/tmp/aquamatic/state.txt"

extern int tui_flag;

void read_pid_file(FILE *pid_file, pid_t *pid);
void run_daemon();
void quit_daemon();
void run_tui();

#endif
