#include "aquamatic.h"

char temperature_buf[512];
float temperature = 0.0;

void write_data(FILE *writer, char *buf)
{
        if (flock(fileno(writer), LOCK_EX) != 0) {
                fclose(writer);
                return;
        }

        char timestamp[20];
        get_timestamp(timestamp, sizeof(timestamp));
        fprintf(writer, "%s, %s\n", timestamp, buf);
        fflush(writer);
}

static void parse_serial_buffer(const char *restrict __src, char *restrict __dest,
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
                                buf_pos = 0; // overflow safety
                        }
                }
        }
}

/* communication via usb_port */
void get_ph(int usb_port, char *local_buf)
{
        int bytes_read = read(usb_port, local_buf, sizeof(local_buf));
        if (bytes_read > 0) {
                local_buf[bytes_read] = '\0';
        } else if (bytes_read < 0) {
                if (errno == EAGAIN)
                        return;
                perror("Error reading from USB port");
        }

}

/* Assumes communication directly via rx and tx pins of the pi. */
void get_temperature(int serial_port, char *local_buf)
{
        int num_bytes = read(serial_port, local_buf, sizeof(local_buf) - 1);
        if (num_bytes > 0) {
                parse_serial_buffer(local_buf, temperature_buf, sizeof(temperature_buf) - 1, num_bytes);
        } else if (num_bytes < 0) {
                if (errno == EAGAIN) {
                    return;
                }
                perror("Error reading from serial port");
        }
}

/*
Reads the latest temperature from file written as (TIMESTAMP TEMPERATURE)
and formatted as (%s %f). Rewinding is handled.
*/
static int read_temperature(FILE *reader, char *buf)
{
        persistent int last_pos = 0;
        fseek(reader, last_pos, SEEK_SET);
        char line[100];
        char last_line[100];

        unsigned int i = 0;
        int new_data = 0;
        while (fgets(line, sizeof(line), reader)) {
                strcpy(last_line, line);
                i++;
                new_data = 1;
        }

        if (!new_data) {
                return 0;
        }
        last_pos = ftell(reader);

        if (i >= 10000) {
                truncate(TEMP_LOG, 0);
                freopen(TEMP_LOG, "w", reader);
                fprintf(reader, "%s", last_line);
                fflush(reader);
                last_pos = 0;
        }

        char *token = strrchr(last_line, ',');
        if (!token) {
                fprintf(stderr, "Error parsing Temp Data\n");
                strcpy(buf, "");
                return -2;
        }
        // printf("token: %s\n", token + 1);
        strcpy(buf, token + 1);
        return 0;
}

void read_temperature_from_file(char *buf)
{
        FILE *reader = fopen("/tmp/aquamatic/temperature.txt", "r");
        read_temperature(reader, buf);
        return;
}
