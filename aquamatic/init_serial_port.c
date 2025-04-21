#include <termios.h>

#include "aquamatic.h"

int init_serial_port(int serial_port)
{
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

        return 0;
}
