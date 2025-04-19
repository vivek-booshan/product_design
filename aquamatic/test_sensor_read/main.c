#include "tsensor.h"

int main(void)
{
        FILE *reader = fopen(TEMP_LOG, "r");
        FILE *writer = fopen(TEMP_LOG, "a");
        char local_buf[512];
        char local_temp_buf[512];

        int serial_port = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
        if (init_serial_port(serial_port) != 0)
                return 1;

        while (1) {
                get_temperature(serial_port, local_temp_buf);
                write_temperature(writer, temperature_buf);
                read_temperature(reader, local_buf);
                printf("local_buf: %s\n", local_buf);
                sleep(1);
        }

        fclose(reader);
        close(serial_port);
        return 0;
}


