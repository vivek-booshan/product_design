#include "tsensor.h"

int main(void)
{
        int serial_port = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
        if (init_serial_port(serial_port) != 0)
                return 1;

        pthread_t writer_thread, reader_thread;
        pthread_create(&writer_thread, NULL, serial_writer, &serial_port);
        pthread_create(&reader_thread, NULL, file_reader, NULL);

        pthread_join(writer_thread, NULL);
        pthread_join(reader_thread, NULL);

        // FILE *reader = fopen(TEMP_LOG, "r");
        // FILE *writer = fopen(TEMP_LOG, "a");
        // char local_buf[512];
        // char local_temp_buf[512];


        // while (1) {
        //         get_temperature(serial_port, local_temp_buf);
        //         write_temperature(writer, temperature_buf);
        //         read_temperature(reader, local_buf);
        //         printf("local_buf: %s\n", local_buf);
        //         sleep(1);
        // }

        // fclose(reader);
        close(serial_port);
        return 0;
}

        // close(serial_port);


