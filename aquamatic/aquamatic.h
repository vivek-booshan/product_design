#ifndef AQUAMATIC_H
#define AQUAMATIC_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <ncurses.h>

#ifndef cast
#define cast(Type) (Type)
#endif

#define AQUA_DIR "/tmp/aquamatic"
#define PID_FILE "/tmp/aquamatic/daemon.pid"
#define STATE_FILE "/tmp/aquamatic/state.txt"

extern volatile int tui_flag;

#define NUM_OPTIONS 9
extern const char *menu_options[NUM_OPTIONS]; // declared in tui.c

void read_pid_file(FILE *pid_file, pid_t *pid);
void run_daemon(void);
void quit_daemon(void);
void run_tui(void);
void tui_signal_handler(int signum);
void get_timestamp(char *buffer, size_t buffer_size);
void show_preset_menu(void);
void print_help(const char *program_name);
void get_temperature(void);
void display_temperature(void);

// #ifdef AQUA_IMPLEMENTATION

// #include <stdint.h>
// #include <unistd.h>
// #include <signal.h>
// #include <fcntl.h>
// #include <sys/stat.h>
// #include <sys/file.h>
// #include <errno.h>
// #include <time.h>
// #include <ncurses.h>



// static void tui_signal_handler(int signum)
// {
//         cast(void) signum;
//         tui_flag = 1;
// }

// static inline void read_pid_file(FILE *pid_file, pid_t *pid)
// {
//         if (fscanf(pid_file, "%d", pid) != 1) {
//         printf("Error reading PID file.\n");
//         fclose(pid_file);
//         return;
//     }
// }

// static inline void ensure_directory_exists(const char *path)
// {
//         if (mkdir(path, 0755) < 0 && errno != EEXIST) {
//                 perror("Failed to create PID directory");
//                 exit(EXIT_FAILURE);
//         }
// }

// static void get_pid(pid_t *pid)
// {
//         FILE *file = fopen(PID_FILE, "r");
//         if (!file) {
//                 printf("No running daemon found.\n");
//                 return;
//         }
//         read_pid_file(file, pid);
//         fclose(file);
// }

// void run_daemon(void)
// {
//         signal(SIGUSR1, tui_signal_handler);

//         ensure_directory_exists(AQUA_DIR);

//         int pid_fd = open(PID_FILE, O_CREAT | O_RDWR, 0666);
//         if (pid_fd < 0) {
//                 perror("Failed to open PID file");
//                 exit(EXIT_FAILURE);
//         }

//         int state_fd = open(STATE_FILE, O_CREAT | O_RDWR, 0666);
//         if (state_fd < 0) {
//                 perror("Failed to open STATE file");
//                 exit(EXIT_FAILURE);
//         }

//         // Try to lock the file
//         if (flock(pid_fd, LOCK_EX | LOCK_NB) != 0) {
//                 printf("Daemon is already running.\n");
//                 exit(EXIT_FAILURE);
//         }

//         pid_t pid = fork();
//         if (pid < 0) exit(EXIT_FAILURE);
//         if (pid > 0) exit(EXIT_SUCCESS); // Parent exits, daemon continues

//         setsid();  // Create a new session

//         pid = fork(); // guarantee independence of daemon --> block accidental terminal control
//         if (pid < 0) exit(EXIT_FAILURE);
//         if (pid > 0) exit(EXIT_SUCCESS); // First child exits, daemon continues

//         // Save daemon PID to file
//         dprintf(pid_fd, "%d\n", getpid());

//         umask(0);
//         // chdir("/");  // Change working directory to root

//         close(STDIN_FILENO);
//         close(STDOUT_FILENO);
//         close(STDERR_FILENO);

//         // Daemon: periodically check for updates
//         // int i = 0;
//         while (1) {
//                 if (tui_flag)  {
//                     // printf("Daemon successfully received user input from TUI\n");
//                     FILE *file = fopen(STATE_FILE, "a");
//                         if (file) {
//                         // int state;
//                         // if (fscanf(file, "%d", &state) == 1) {
//                             // Daemon uses the state value
//                                 char buffer[20];
//                                 get_timestamp(buffer, sizeof(buffer));
//                                 fprintf(file, "[%s] Daemon received update", buffer);
//                             // printf("Daemon running with state: %d\n", state);
//                         }
//                         fclose(file);
//                         printf("%d", tui_flag);
//                         tui_flag = 0; // reset flag
//                 }

//                 // fprintf();
//                 sleep(1);  // Check every second
//         }
// }

// void quit_daemon(void)
// {
//         pid_t pid;
//         get_pid(&pid);

//         if (kill(pid, SIGTERM) != 0) {
//                 if (errno == ESRCH) {
//                         // ESRCH: No such process
//                         printf("Process with PID %d not found.\n", pid);
//                 } else if (errno == EPERM) {
//                         // EPERM: Operation not permitted (e.g., lack of permission)
//                         printf("Permission denied: Unable to terminate process with PID %d.\n", pid);
//                 } else {
//                         // Other error
//                         perror("Failed to terminate daemon");
//                 }
//         } else {
//                 printf("\nDaemon (PID %d) terminated.\n", pid);
//                 unlink(PID_FILE);  // Remove PID file
//         }
// }

// static inline void get_timestamp(char *buffer, size_t buffer_size)
// {
//         time_t rawtime;
//         struct tm *timeinfo;

//         time(&rawtime);               // Get current time
//         timeinfo = localtime(&rawtime); // Convert to local time

//         // Format the time as "YY-MM-DD HH:MM:SS"
//         strftime(buffer, buffer_size, "%y-%m-%d %H:%M:%S", timeinfo);
// }

// void doublecheck_kill_daemon(void) {
    
//         clear();
//         mvprintw(10, 4, "Are you sure you want to kill the daemon? (y/n)");
//         refresh();

//         int ch = getch();
//         if (ch == 'y' || ch == 'Y') {
//                 // mvprintw(12, 4, "");
//                 quit_daemon();
//                 mvprintw(12, 4, "Daemon terminated.");
//         } else {
//                 mvprintw(12, 4, "Operation canceled.");
//         }

//         refresh();
//         getch();
// }


// static void show_sensor_menu(const char *title) {
//     int highlight = 0;
//     int choice = -1;
//     int ch;
//     const char *options[] = {
//         "View Current Value",
//         "Set target",
//         "Back"
//     };
//     const int num_options = 3;

//     // Initialize ncurses
//     initscr();
//     noecho();
//     cbreak();
//     curs_set(0);  // Hide cursor
//     keypad(stdscr, TRUE); // Enable arrow keys

//     // Define window size
//     int height = 10, width = 40;
//     int start_y = (LINES - height) / 2;
//     int start_x = (COLS - width) / 2;

//     // Create window
//     WINDOW *menu_win = newwin(height, width, start_y, start_x);
//     box(menu_win, 0, 0); // Draw border

//     refresh();

//     while (1) {
//         werase(menu_win);
//         box(menu_win, 0, 0); // Redraw the border

//         // Center title correctly inside [ ]
//         int title_length = strlen(title); // + 4;  // Account for "[ ]"
//         int title_x = (width - title_length) / 2;
//         mvwprintw(menu_win, 1, title_x, "%s", title);

//         // Instructions for user
//         // mvwprintw(menu_win, 2, 2, "Use up/down arrows to navigate, ENTER to select.");

//         // Print menu options, properly centered
//         for (int i = 0; i < num_options; i++) {
//             int option_x = (width - strlen(options[i])) / 2; // Center horizontally
//             if (i == highlight) wattron(menu_win, A_REVERSE);
//             mvwprintw(menu_win, 4 + i, option_x, "%s", options[i]);
//             if (i == highlight) wattroff(menu_win, A_REVERSE);
//         }

//         wrefresh(menu_win);
//         ch = getch();

//         switch (ch) {
//             case KEY_UP:
//                 highlight = (highlight == 0) ? num_options - 1 : highlight - 1;
//                 break;
//             case KEY_DOWN:
//                 highlight = (highlight == num_options - 1) ? 0 : highlight + 1;
//                 break;
//             case 10:  // Enter key
//                 choice = highlight;
//                 break;
//             case 'q':
//                 delwin(menu_win);
//                 endwin();
//                 return;
//         }

//         if (choice != -1) {
//             werase(menu_win);
//             box(menu_win, 0, 0);

//             // Display action result, centered
//             const char *message;
//             switch (choice) {
//                 case 0: message = "Viewing current value..."; break;
//                 case 1: message = "Threshold setting coming soon!"; break;
//                 case 2: 
//                     delwin(menu_win);
//                     endwin();
//                     return;
//             }

//             mvwprintw(menu_win, height / 2, (width - strlen(message)) / 2, "%s", message);
//             mvwprintw(menu_win, height - 2, (width - 24) / 2, "Press any key to return...");
//             wrefresh(menu_win);
//             getch();
//             choice = -1;
//         }
//     }

//     delwin(menu_win);
//     endwin();
// }

// const char *menu_options[NUM_OPTIONS] = { // declared in aquamatic.h
//     // "Start Daemon",
//     // "Kill Daemon",
//     "Temperature",
//     "pH",
//     "kH (coming soon)",
//     "gH (coming soon)",
//     "Salinity",
//     "Nitrates/Nitrites/Ammonia (coming soon)",
//     "Calcium/Magnesium (coming soon)",
//     "Presets",
//     "Update"
// };
// void run_tui(void) {
//         FILE *pid_file = fopen(PID_FILE, "r");
//         pid_t pid;

//         if (!pid_file) {
//                 printf("No running daemon found.\n");
//                 //     // printf("Would you like to start aquamatic? (y/n)");
//                 //     // int start_session = getchar();
//                 //     // if (start_session == 'y') {
//                 //     //     printf("Starting Daemon");
//                 //     //     run_daemon();
//                 //     // } else if (start_session == 'n')
//                 return;
//         }
//         read_pid_file(pid_file, &pid);
//         fclose(pid_file);

//         initscr();
//         noecho();
//         curs_set(FALSE);
//         keypad(stdscr, TRUE); // Enable arrow key input

//         int highlight = 0;
//         int choice = -1;
//         int ch;

//         while (1) {
//                 clear();
//                 mvprintw(2, 2, "Aquamatic Control Panel");
//                 mvprintw(3, 2, "Use arrow keys to navigate, ENTER to select, 'q' to quit.");

//                 // Print menu options
//                 for (int i = 0; i < NUM_OPTIONS; i++) {
//                         if (i == highlight)  {
//                                 attron(A_REVERSE);
//                         }
//                         mvprintw(5 + i, 4, "%s", menu_options[i]);
//                         if (i == highlight) { 
//                                 attroff(A_REVERSE);
//                         }
//                 }

//                 ch = getch();

//                 switch (ch) {
//                 case KEY_UP:
//                         highlight = (highlight == 0) ? NUM_OPTIONS - 1 : highlight - 1;
//                         break;
//                 case KEY_DOWN:
//                         highlight = (highlight == NUM_OPTIONS - 1) ? 0 : highlight + 1;
//                         break;
//                 case 10: // Enter key
//                         choice = highlight;
//                         break;
//                 case 'q': //fallthrough
//                 case 'Q':
//                         endwin();
//                         return;
//                 }

//                 // Handle menu selection
//                 if (choice != -1) {
//                         clear();
//                         switch (choice) {
//                         // case -2:
//                         //     run_daemon();
//                         //     mvprintw(10, 4, "Daemon started.");
//                         //     break;
//                         // case -1:
//                         //     doublecheck_kill_daemon();
//                         //     // mvprintw(10, 4, "Daemon (PID %d) terminated.", pid);
//                         //     break;
//                         case 0:
//                             show_sensor_menu("Temperature");
//                             // mvprintw(10, 4, "Temperature: %.2f°C", get_temperature());
//                             break;
//                         case 1:
//                             show_sensor_menu("pH");
//                             // mvprintw(10, 4, "pH Level: %.2f", get_ph());
//                             break;
//                         case 2:
//                             // show_sensor_menu("kH");
//                             // mvprintw(10, 4, "Salinity: %.2f PSU", get_salinity());
//                             break;
//                         case 3:
//                             // show_sensor_menu("gH");
//                             break;
//                         case 4:
//                             show_sensor_menu("Salinity");
//                             // show_sensor_menu("Salinity");
//                             break;
//                         case 5:
//                             // show_sensor_menu("Ammonia/Nitrites/Nitrates");
//                             break;
//                         case 6:
//                             // show_sensor_menu("Calcium/Magnesium");
//                             break;
//                         case 7:
//                                 show_preset_menu();
//                                 break;
//                         case 8:
//                                 if (kill(pid, SIGUSR1) != 0) {
//                                         mvprintw(10, 4, "Failed to send signal to daemon");
//                                 } else {
//                                         mvprintw(10, 4, "Signal sent to daemon");
//                                 }
//                                 // signal(SIGUSR1, tui_signal_handler);
//                                 break;
//                         }

//                         mvprintw(12, 4, "Press any key to return...");
//                         refresh();
//                         getch(); // Wait for user input before returning
//                         choice = -1;
//                 }
//         }

//         endwin();  // End ncurses
//         printf("Sending User Input to Daemon");
// }

// #define PRESET_OPTIONS 10
// const char *preset_menu_options[PRESET_OPTIONS] = {
//     "Africa: African Cichlids (Lake Malawi, Lake Tanganyika), etc.",
//     "South America: Discus, Angelfish, Apistogramma, Tetras, etc.",
//     "Southeast Asia: Betta, Gourami, Rasboras, Loaches, etc.",
//     "North America: Sunfish, Rainbow Shiners, Darters, Native Catfish, etc.",
//     "Central America: Firemouth, Convicts, Jack Dempsey, etc.",
//     "Tropical: Tetras, Barbs, Rasboras, Corydoras, etc.",
//     "Coldwater: Goldfish, White Cloud Minnows, etc.",
//     "Marine: Reef Fish, Clownfish, Wrasses, etc.",
//     "Brackish: Mudskippers, Archerfish, Mollies, etc.",
//     "Custom",
// };

// static void show_preset_menu(void) {
//         int highlight = 0;
//         int choice = -1;
//         int ch;

//         while (1) {
//                 clear();
//                 mvprintw(2, 2, "Presets Menu");
//                 mvprintw(3, 2, "Use arrow keys to navigate, ENTER to select, 'q' to quit.");

//                 // Print menu options
//                 for (int i = 0; i < PRESET_OPTIONS; i++) {
//                         if (i == highlight) 
//                                 attron(A_REVERSE);
//                         mvprintw(5 + i, 4, "%s", preset_menu_options[i]);
//                         // if (i == highlight) {
//                         // attroff(A_REVERSE);
//                         // }
//                 }

//                 ch = getch();

//                 switch (ch) {
//                 case KEY_UP:
//                         highlight = (highlight == 0) ? PRESET_OPTIONS - 1 : highlight - 1;
//                         break;
//                 case KEY_DOWN:
//                         highlight = (highlight == PRESET_OPTIONS - 1) ? 0 : highlight + 1;
//                         break;
//                 case 10: // Enter key
//                         choice = highlight;
//                         break;
//                 case 'q': //fallthrough
//                 case 'Q':
//                         endwin();
//                         return;
//                 }

//                 // Handle menu selection
//                 if (choice != -1) 
//                 switch (choice) {
//                 case 0:
//                     break;
//                 case 1:
//                     break;
//                 case 2:
//                     break;
//                 case 3:
//                     break;
//                 case 4:
//                     break;
//                 case 5:
//                     break;
//                 case 6:
//                     break;
//                 case 7:
//                     break;
//                 case 8:
//                     break;
//                 case 9:
//                     break;
//                 } 
                

//         }
// }

// #endif // AQUA_IMPLEMENTATION

#endif // AQUAMATIC_H
