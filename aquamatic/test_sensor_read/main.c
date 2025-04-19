#include "tsensor.h"
#define uS_TO_S 1000000

#define TEMP_LOG "/tmp/aquamatic/temperature.txt"

int main(void)
{
        int serial_port = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
        int ok = init_serial_port(serial_port);
        if (ok != 0) return 1;
        FILE *file = fopen(TEMP_LOG, "a");
        if (file == NULL)
                return 1;
        while (1) {
                char local_buf[512];
                memset(&local_buf, '\0', sizeof(local_buf));
                get_temperature(serial_port, local_buf);
                write_temperature(file, temperature_buf);
                read_temperature(file, temperature_buf);
                printf("%s\n", temperature_buf);       
                usleep(uS_TO_S);
        }
        fclose(file);
        close(serial_port);
        return 0;
}


