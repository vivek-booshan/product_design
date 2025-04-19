#include "aquamatic.h"

void get_timestamp(char *buffer, size_t buffer_size)
{
        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);               // Get current time
        timeinfo = localtime(&rawtime); // Convert to local time

        // Format the time as "YY-MM-DD HH:MM:SS"
        strftime(buffer, buffer_size, "%y-%m-%d %H:%M:%S", timeinfo);
}
