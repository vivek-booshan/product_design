#include "pthread.h"
#include "tsensor.h"
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <time.h>

pthread_mutex_t temp_lock = PTHREAD_MUTEX_INITIALIZER;
char temperature_buf[512];
float temperature = 0.0;

static inline void get_timestamp(char *buffer, size_t buffer_size)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime); 

    strftime(buffer, buffer_size, "%y-%m-%d %H:%M:%S", timeinfo);
}

void write_temperature(FILE *file, const char *temperature)
{
        if (flock(fileno(file), LOCK_EX) != 0) {
                perror("Error Locking file");
                fclose(file);
                return;
        }
        char timestamp[20];
        get_timestamp(timestamp, sizeof(timestamp));
        fprintf(file, "%s, %s\n", timestamp, temperature);
        fflush(file);
}

static void parse_temperature(const char *restrict __src, char *restrict __dest,
                              const size_t __destsize, const int num_bytes)
{
        int buf_pos = 0;
        for (int i = 0; i < num_bytes; i++) {
                char c = __src[i];
                if (c == '\n' || c == '\r') {
                        __dest[buf_pos] = '\0';
                        buf_pos = 0;
                } else {
                        if (buf_pos < __destsize) {
                                __dest[buf_pos++] = c;
                        } else {
                                // Overflow safety
                                buf_pos = 0;
                        }
                }
        }
}

void get_temperature(int serial_port, char *local_buf)
{
        int num_bytes = read(serial_port, local_buf, sizeof(local_buf) - 1);

        if (num_bytes > 0) {
                parse_temperature(local_buf, temperature_buf, sizeof(temperature_buf) - 1, num_bytes);
        } else if (num_bytes < 0) {
                if (errno == EAGAIN) {
                    return;
                }
                perror("Error reading from serial port");
        }
}
