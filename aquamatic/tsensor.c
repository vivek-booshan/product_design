#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <termios.h>

#define I2C_ADDR 0x66
#define MAX_LEN 20

static int open_i2c(void)
{
        int i2c_fd = open("/dev/i2c-1", O_RDWR);
        if (i2c_fd < 0) {
                perror("I2C open error");
                exit(1);
        }

        if (ioctl(i2c_fd, I2C_SLAVE, I2C_ADDR) < 0) {
                perror("I2C ioctl error");
                close(i2c_fd);
                exit(1);
        }

        return i2c_fd;
}

static int open_serial(void)
{
        int serial_fd = open("/dev/serial0", O_RDWR | O_NOCTTY | O_SYNC);
        if (serial_fd < 0) {
                perror("Serial open error");
                exit(1);
        }

        struct termios tty;
        memset(&tty, 0, sizeof(tty));
        if (tcgetattr(serial_fd, &tty) != 0) {
                perror("tcgetattr error");
                exit(1);
        }

        cfsetospeed(&tty, B9600);
        cfsetispeed(&tty, B9600);
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;    
        tty.c_iflag = 0;                
        tty.c_oflag = 0;                
        tty.c_lflag = 0;                
        tty.c_cc[VMIN]  = 1;            
        tty.c_cc[VTIME] = 1;            

        if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
                perror("tcsetattr error");
                exit(1);
        }

        return serial_fd;
}

static inline void to_lowercase(char *str)
{
        for (; *str; ++str) *str = tolower(*str);
}

int main(void)
{
        int i2c_fd = open_i2c();
        int serial_fd = open_serial();

        char buffer[MAX_LEN];
        char rtd_data[MAX_LEN];
        int time_delay;
        char code;

        while (1) {
                int bytes_read = read(serial_fd, buffer, MAX_LEN - 1);
                if (bytes_read > 0) {
                        buffer[bytes_read] = '\0';

                        // Remove trailing CR or LF
                        char *newline = strchr(buffer, '\r');
                        if (newline) *newline = '\0';
                        newline = strchr(buffer, '\n');
                        if (newline) *newline = '\0';

                        to_lowercase(buffer);

                        time_delay = (buffer[0] == 'c' || buffer[0] == 'r') ? 600 : 250;

                        // Write to I2C
                        if (write(i2c_fd, buffer, strlen(buffer)) != strlen(buffer)) {
                                perror("I2C write failed");
                        }

                        if (strcmp(buffer, "sleep") != 0) {
                                usleep(time_delay * 1000);

                                // Request 20 bytes from I2C
                                if (read(i2c_fd, &code, 1) != 1) {
                                    perror("I2C read failed (code)");
                                    continue;
                                }

                                switch (code) {
                                case 1: printf("Success\n"); break;
                                case 2: printf("Failed\n"); break;
                                case 254: printf("Pending\n"); break;
                                case 255: printf("No Data\n"); break;
                                default: printf("Unknown code: %d\n", code);
                                }

                                // Read rest of data
                                int r = read(i2c_fd, rtd_data, MAX_LEN);
                                if (r > 0) {
                                    rtd_data[r] = '\0';
                                    printf("%s\n", rtd_data);
                                }
                        }
                }

        usleep(100000);  // 100ms loop delay
        }

    close(i2c_fd);
    close(serial_fd);
    return 0;
}
