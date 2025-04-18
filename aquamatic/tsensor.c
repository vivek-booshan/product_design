#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

#define SERIAL_PORT "/dev/serial0"
#define BAUD_RATE B9600

int main(void)
{
        int serial_port = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
        if (serial_port < 0) {
                perror("Error opening serial port");
                return 1;
        }

        struct termios tty;
        if (tcgetattr(serial_port, &tty) != 0) {
                perror("Error getting terminal attributes");
                close(serial_port);
                return 1;
        }

        cfsetispeed(&tty, BAUD_RATE);
        cfsetospeed(&tty, BAUD_RATE);

        tty.c_cflag &= ~PARENB; // no parity
        tty.c_cflag &= ~CSTOPB; // 1 stop bit
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;

        tty.c_cflag &= ~CRTSCTS; // no flow ctrl
        tty.c_cflag |= CREAD | CLOCAL;

        tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // raw input
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // no software flow ctrl
        tty.c_oflag &= ~OPOST; // raw output

        /* set timeout (non block read) */
        tty.c_cc[VMIN] = 0;
        tty.c_cc[VTIME] = 10; // timeout in 100ms incr

        if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
                perror("Error setting terminal attributes");
                close(serial_port);
                return 1;
        }

        printf("Listening on %s...\n", SERIAL_PORT);

        char read_buf[512];
        memset(&read_buf, '\0', sizeof(read_buf));

        while (1) {
                int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
                if (num_bytes < 0) {
                        if (errno == EAGAIN) continue;
                        perror("Error reading from serial port");
                        break;
                } else if (num_bytes > 0) {
                        read_buf[num_bytes] = '\0';
                        printf("%s", read_buf);

                }
                usleep(100000); // 100ms
        }
        close(serial_port);
        return 0;
}
