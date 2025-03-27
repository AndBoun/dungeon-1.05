//
// Created by Andrew Boun on 3/24/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#include <dungeon.h>
#include <ncurses_ui.h>

int handle_monster_list(Dungeon *d);
static int print_monster_list(Dungeon *d, Monster alive[], int scroll);

int handle_monster_list(Dungeon *d){

    // Create array with only alive monsters for easier scrolling
    Monster alive [d->num_monsters_alive];
    for (int i = 0; i < d->num_monsters; i++){
        if (d->monsters[i].alive) {
            alive[i] = d->monsters[i];
        }
    }

    int input;
    int scroll = 0;
    do {
        print_monster_list(d, alive, scroll);

        timeout(-1);
        input = getch();

        switch (input) {
            case 'q': // quit
                destroy_ncurses();
                printf("Game terminated by user\n");
                exit(0);
                break;
                
            case KEY_DOWN: // Scroll down
                scroll = print_monster_list(d, alive, ++scroll);
                break;

            case KEY_UP: // Scroll up
                scroll = print_monster_list(d, alive, --scroll);
                break;

            default:
                break;
        }
    } while (input != 27); // 27 is the ASCII value for ESC key

    clear();
    render_grid(d);
    return 1;
}

// Prints monst list, symbol, and position relative to player
// return position of scroll
static int print_monster_list(Dungeon *d, Monster alive[], int scroll){
    clear();
    int i;
    int j = 2; // current line to be printed at
    char monster_symbol;

    // Check if the scroll is out of bounds
    if (d->num_monsters_alive - scroll < 10) return d->num_monsters_alive - 10;
    if (scroll < 0) return 0;
    
    // Print the header
    mvprintw(1, 1, "Monster");
    mvprintw(1, 35, "X-Position");
    mvprintw(1, 69, "Y-Position");
    
    int c = 0; // counter number of lines, max of 10 lines

    for (i = scroll; i < d-> num_monsters_alive && c < 10; i++){
        monster_symbol= alive[i].symbol;

            mvprintw(j, 1, "%c", monster_symbol);

            int relative_x = alive[i].x - d->pc.x;
            int relative_y = alive[i].y - d->pc.y;

            if (relative_x < 0) {
                relative_x = -relative_x;
                mvprintw(j, 35, "%d West", relative_x);
            } else if (relative_x == 0) {
                mvprintw(j, 35, "Here", relative_x);
            }
            else {
                mvprintw(j, 35, "%d East", relative_x);
            }

            if (relative_y < 0) {
                relative_y = -relative_y;
                mvprintw(j, 69, "%d North", relative_y);
            } else if (relative_y == 0) {
                mvprintw(j, 69, "Here", relative_y);
            }
            else {
                mvprintw(j, 69, "%d South", relative_y);
            }
        
        c++; // increment line counter
        j++; // increment current line
    }

    refresh();
    return scroll;
}