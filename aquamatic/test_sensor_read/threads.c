#include "stdio.h"
#include "tsensor.h"
#include <string.h>
#include <sys/file.h>

pthread_mutex_t temp_lock = PTHREAD_MUTEX_INITIALIZER;

/*
reads temperature data from device serial and writes it to the TEMP_LOG file.
*/
void *serial_writer(void *arg)
{
        int serial_port = *cast(int *, arg);
        FILE *writer = fopen(TEMP_LOG, "a");
        if (!writer) {
                perror("failed to open writer");
                return NULL;
        }
        while (1) {
                char local_buf[512];
                memset(&local_buf, '\0', sizeof(local_buf));
                pthread_mutex_lock(&temp_lock);
                get_temperature(serial_port, local_buf);
                write_temperature(writer, temperature_buf);
                pthread_mutex_unlock(&temp_lock);
                sleep(1);
        }
        fclose(writer);
        return NULL;
}

void *file_reader(void *arg)
{
        FILE *reader = fopen(TEMP_LOG, "r");
        if (!reader) {
                perror("failed to open reader");
                return NULL;
        }

        while (1) {
                char local_buf[512];
                pthread_mutex_lock(&temp_lock);
                read_temperature(reader, local_buf);
                pthread_mutex_unlock(&temp_lock);
                printf("From File: %s\n", local_buf);
                sleep(1);
        }
        fclose(reader);
        return NULL;        
}

