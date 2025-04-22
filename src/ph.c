#include "../include/aquamatic.h"

#define MAX_LINE_LEN 256

/*
Assumes communication via usb port. Due to the serial communication being
non-atomic, partial reads can occurs. Therefore, an internal buffer is first
used to get the full data (currently an expensive implementation) and then that
is copied to the external buffer.
*/
void get_ph(int usb_port, char *line_buf)
{
    static char internal_buf[MAX_LINE_LEN] = {0};
    static int buf_pos = 0;

    char temp[64];
    int bytes_read = read(usb_port, temp, sizeof(temp) - 1);

    if (bytes_read > 0) {
        temp[bytes_read] = '\0';

        // Append to internal buffer
        for (int i = 0; i < bytes_read && buf_pos < MAX_LINE_LEN - 1; ++i) {
            internal_buf[buf_pos++] = temp[i];
            if (temp[i] == '\n') {
                internal_buf[buf_pos] = '\0';
                strncpy(line_buf, internal_buf, MAX_LINE_LEN);
                buf_pos = 0;
                return;
            }
        }

    } else if (bytes_read < 0 && errno != EAGAIN) {
        perror("Error reading from USB port");
    }

    // No full line yet
    line_buf[0] = '\0';
}
