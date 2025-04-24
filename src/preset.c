#include <ncurses.h>

#include "../include/aquamatic.h"

#define PRESET_OPTIONS 10

setpoint_t sp = {30.0, 7.0};

const char *preset_menu_options[PRESET_OPTIONS] = {
    "Africa: African Cichlids (Lake Malawi, Lake Tanganyika), etc.",
    "South America: Discus, Angelfish, Apistogramma, Tetras, etc.",
    "Southeast Asia: Betta, Gourami, Rasboras, Loaches, etc.",
    "North America: Sunfish, Rainbow Shiners, Darters, Native Catfish, etc.",
    "Central America: Firemouth, Convicts, Jack Dempsey, etc.",
    "Tropical: Tetras, Barbs, Rasboras, Corydoras, etc.",
    "Coldwater: Goldfish, White Cloud Minnows, etc.",
    "Marine: Reef Fish, Clownfish, Wrasses, etc.",
    "Brackish: Mudskippers, Archerfish, Mollies, etc.",
    "Custom",
};

void show_preset_menu(void) {
    int highlight = 0;
    int choice = -1;
    int ch;

    while (1) {
        clear();
        mvprintw(2, 2, "Presets Menu");
        mvprintw(3, 2, "Use arrow keys to navigate, ENTER to select, 'q' to quit.");

        // Print menu options
        for (int i = 0; i < PRESET_OPTIONS; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(5 + i, 4, "%s", preset_menu_options[i]);
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }

        ch = getch();

        switch (ch) {
            case KEY_UP:
                highlight = (highlight == 0) ? PRESET_OPTIONS - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == PRESET_OPTIONS - 1) ? 0 : highlight + 1;
                break;
            case 10: // Enter key
                choice = highlight;
                break;
            case 'q': //fallthrough
            case 'Q':
                endwin();
                return;
        }

        // Handle menu selection
        if (choice != -1) {
            switch (choice) {
                case 0:
                    sp.temp = 26;
                    sp.ph = 8;
                    break;
                case 1:
                    sp.temp = 27;
                    sp.ph = 8.5;
                    break;
                case 2:
                    sp.temp = 27;
                    sp.ph = 6.5;
                    break;
                case 3:
                    sp.temp = 21;
                    sp.ph = 7.5;
                    break;
                case 4:
                    sp.temp = 26;
                    sp.ph = 7.2;
                    break;
                case 5:
                    sp.temp = 26;
                    sp.ph = 6.8;
                    break;
                case 6:
                    sp.temp = 20;
                    sp.ph = 7.2;
                    break;
                case 7:
                    sp.temp = 25;
                    sp.ph = 8.3;
                    break;
                case 8:
                    sp.temp = 25;
                    sp.ph = 8.3;
                    break;
                case 9:
                    break;
            } 
        }
    
    }
    endwin();
}
