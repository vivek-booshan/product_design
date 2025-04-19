#include <fcntl.h>
#include <string.h>

#include "aquamatic.h"

#define SERIAL_PORT "/dev/serial0"
#define BAUD_RATE B9600

pthread_mutex_t temp_lock;
char temperature_buf[512] = "00.0000";


static void init_serial_port(int serial_port, struct termios *tty)
{
        serial_port = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
        if (serial_port < 0) {
                perror("Error opening serial port");
                return;
        }

        // struct termios tty;
        if (tcgetattr(serial_port, tty) != 0) {
                perror("Error getting terminal attributes");
                close(serial_port);
                return;
        }

        cfsetispeed(tty, BAUD_RATE);
        cfsetospeed(tty, BAUD_RATE);

        tty->c_cflag &= ~PARENB; // no parity
        tty->c_cflag &= ~CSTOPB; // 1 stop bit
        tty->c_cflag &= ~CSIZE;
        tty->c_cflag |= CS8;

        tty->c_cflag &= ~CRTSCTS; // no flow ctrl
        tty->c_cflag |= CREAD | CLOCAL;

        tty->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // raw input
        tty->c_iflag &= ~(IXON | IXOFF | IXANY); // no software flow ctrl
        tty->c_oflag &= ~OPOST; // raw output

        /* ->et timeout (non block read) */
        tty->c_cc[VMIN] = 0;
        tty->c_cc[VTIME] = 10; // timeout in 100ms incr

        if (tcsetattr(serial_port, TCSANOW, tty) != 0) {
                perror("Error setting terminal attributes");
                close(serial_port);
                return;
        }
}

void get_temperature(void)
{
        int serial_port;
        struct termios tty;
        init_serial_port(serial_port, &tty);

        char local_buf[512];
        memset(&local_buf, '\0', sizeof(local_buf));

        while (1) {
                int num_bytes = read(serial_port, &local_buf, sizeof(local_buf));
                printf("Read %d bytes as %s\n", num_bytes, local_buf);
                if (num_bytes < 0) {
                        if (errno == EAGAIN) continue;
                        perror("Error reading from serial port");
                        break;
                } else if (num_bytes > 0) {
                        local_buf[num_bytes] = '\0';
                        pthread_mutex_lock(&temp_lock);
                        strncpy(temperature_buf, local_buf, sizeof(temperature_buf)-1);
                        temperature_buf[sizeof(temperature_buf) - 1] = '\0';
                        pthread_mutex_unlock(&temp_lock);
                }

                usleep(100000);
        }
        return;
}
