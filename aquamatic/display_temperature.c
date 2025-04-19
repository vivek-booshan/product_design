#include "aquamatic.h"
#include "bits/pthreadtypes.h"

extern pthread_mutex_t temp_lock;
extern char temperature_buf[512];

void display_temperature(void)
{
        initscr();
        noecho();       
        cbreak();
        curs_set(0);
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);

        // Define window size
        int height = 10, width = 40;
        int start_y = (LINES - height) / 2;
        int start_x = (COLS - width) / 2;

        // Create window
        WINDOW *temperature_readout_win = newwin(height, width, start_y, start_x);
        box(temperature_readout_win, 0, 0); // Draw border

        while (1) {
                pthread_mutex_lock(&temp_lock);
                char local_buf[512];
                strncpy(local_buf, temperature_buf, sizeof(local_buf) - 1);
                local_buf[sizeof(local_buf) - 1] = '\0';
                pthread_mutex_unlock(&temp_lock);

                wclear(temperature_readout_win);
                mvwprintw(temperature_readout_win, 1, 6, "%s", local_buf);
                wrefresh(temperature_readout_win);

                int ch = getch();
                if (ch == 'q' || ch == 'Q')
                        break;
                usleep(100000);
        }
        delwin(temperature_readout_win);
        endwin();
        return;
}

