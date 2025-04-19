#include <ncurses.h>
#include "aquamatic.h"

static int plot_Temperature(void) {
        int ch;
        while (1) {
                ch = getch();
                if (ch == 'q')
                        return 0;
                // Clear screen using ANSI escape
                printf("\033[2J\033[H");

                // Open gnuplot process
                FILE *gp = popen("gnuplot -persist", "w");
                if (gp == NULL) {
                        perror("popen");
                        return 1;
                }

                // Gnuplot configuration and command
                fprintf(gp,
                        "set terminal dumb 120 30\n"
                        "set xdata time\n"
                        "set timefmt \"%%d-%%m-%%y %%H:%%M:%%S\"\n"
                        "set format x \"%%H:%%M:%%S\"\n"
                        "set grid\n"
                        "set title \"Temperature\"\n"
                        "set xlabel \"Time\"\n"
                        "set ylabel \"Temp (°C)\"\n"
                        "plot '/tmp/aquamatic/temperature.txt' using 1:3 with lines title 'Temp'\n"
                );

                fflush(gp);
                pclose(gp);

                sleep(60);  // Refresh every min
        }

        return 0;
}

plot_t get_plot_function(const char *title)
{
        if (strcmp("Temperature", title) == 0) {
                return plot_Temperature;
        // } else if (strcmp("pH", title) == 0) {
        //         return plot_pH;
        } else {
                return NULL;
        }
}
