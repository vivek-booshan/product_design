#ifndef TSENSOR_H
#define TSENSOR_H
#include <stdio.h>
#include <string.h>
// #include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>

#define SERIAL_PORT "/dev/serial0"
#define BAUD_RATE B9600

extern pthread_mutex_t temp_lock;
extern char temperature_buf[512];
extern float temperature;

void get_temperature(int serial_port, char *local_buf);
void write_temperature(FILE *file, const char *temperature_buf);
int init_serial_port(int serial_port);
#endif
