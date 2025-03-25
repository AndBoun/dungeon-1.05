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

#define COLOR_DEFAULT_ID 1
#define COLOR_PLAYER_ID 2
#define COLOR_STAIR_ID 3
#define COLOR_MONSTER_ID 4


void init_ncurses() {
    initscr();
    raw();                 // Disable line buffering
    keypad(stdscr, TRUE);  // Enable special keys
    noecho();              // Don't echo input characters
    curs_set(0);           // Hide cursor
    start_color();         // Enable colors
    timeout(0);            // Non-blocking input

    init_pair(COLOR_DEFAULT_ID, COLOR_WHITE, COLOR_BLACK);    // Default
    init_pair(COLOR_PLAYER_ID, COLOR_MAGENTA, COLOR_YELLOW);  // Player
    init_pair(COLOR_STAIR_ID, COLOR_GREEN, COLOR_BLACK);      // Cooridor
    init_pair(COLOR_MONSTER_ID, COLOR_CYAN, COLOR_RED);       // Monster
}

void destroy_ncurses() {
    endwin();
}

void render_grid(Dungeon *d) {
    clear();
    
    // Draw the dungeon grid with colors based on cell type
    for (int i = 0; i < DUNGEON_HEIGHT; i++) {
        
        for (int j = 0; j < DUNGEON_WIDTH; j++) {
            char cell_type = d->grid[i][j].type;
            attron(COLOR_PAIR(COLOR_DEFAULT_ID)); 
            
            // Choose color based on cell type
            if (cell_type == PLAYER) {
                attron(COLOR_PAIR(COLOR_PLAYER_ID));
            } else if (cell_type == UP_STAIRS || cell_type == DOWN_STAIRS) {
                attron(COLOR_PAIR(COLOR_STAIR_ID));
            } else if ((cell_type >= '0' && cell_type <= '9') || (cell_type >= 'A' && cell_type <= 'F')) {
                attron(COLOR_PAIR(COLOR_MONSTER_ID));
            }
            
            mvaddch(i, j, cell_type);
        }
    }

    mvhline(DUNGEON_HEIGHT, 0, ACS_HLINE, DUNGEON_WIDTH);
    refresh();
}

// Render game over message
void render_game_over(Dungeon *d) {
    clear();
    
    render_grid(d);
    
    if (!d->pc.alive) {
        int text_len = strlen("Player is dead!");
        mvprintw(DUNGEON_HEIGHT + 2, (DUNGEON_WIDTH - text_len) / 2, "Player is dead!");
    } else if (d->num_monsters_alive == 0) {
        int text_len = strlen("All monsters are dead!");
        mvprintw(DUNGEON_HEIGHT + 2, (DUNGEON_WIDTH - text_len) / 2, "All monsters are dead!");
    }
    
    
    refresh();
    timeout(-1);  // Wait indefinitely for a keypress
    getch();
}