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
#define TEMP_LOG "/tmp/aquamatic/temperature.txt"
#define uS_TO_S 1000000

#ifndef cast
#define cast(Type, Value) (Type)Value
#endif

#define persistent static


extern pthread_mutex_t temp_lock;
extern char temperature_buf[512];
extern float temperature;

void get_temperature(int serial_port, char *local_buf);
void write_temperature(FILE *writer, const char *temperature_buf);
int read_temperature(FILE *reader, char *buf);
int init_serial_port(int serial_port);

void *serial_writer(void *arg);
void *file_reader(void *arg);
#endif
