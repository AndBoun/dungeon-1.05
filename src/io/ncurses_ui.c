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
#define COLOR_SUCCESS_ID 3
#define COLOR_MONSTER_ID 4
#define COLOR_ERROR_ID 5
#define COLOR_WARNING_ID 6

// Function declarations
void init_ncurses();
void destroy_ncurses();
void render_grid(Dungeon *d);
void render_game_over(Dungeon *d);
int handle_player_movement(Dungeon *d, int x, int y);
int handle_monster_list(Dungeon *d);
int get_input(Dungeon *d);


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
    init_pair(COLOR_STAIR_ID, COLOR_GREEN, COLOR_BLACK);      // Stairs and Success
    init_pair(COLOR_MONSTER_ID, COLOR_CYAN, COLOR_RED);       // Monster
    init_pair(COLOR_ERROR_ID, COLOR_RED, COLOR_BLACK);        // ERROR
    init_pair(COLOR_WARNING_ID, COLOR_YELLOW, COLOR_BLACK);   // Warning
}

void destroy_ncurses() {
    endwin();
}

void render_top_bar(const char *message, int color_id) {
    // Clear the top bar
    move(0, 0);
    clrtoeol();
    
    // Set color and print message
    attron(COLOR_PAIR(color_id));
    mvprintw(0, (DUNGEON_WIDTH - strlen(message)) / 2, "%s", message);
    attroff(COLOR_PAIR(color_id));
    
    refresh();
}

void render_grid(Dungeon *d) {
    // clear();
    
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
            
            mvaddch(i + 1, j + 1, cell_type);
        }
    }

    refresh();
}

// Render game over message
void render_game_over(Dungeon *d) {
    clear();
    
    render_grid(d);
    
    if (!d->pc.alive) {
        int text_len = strlen("Player is dead!");
        mvprintw(0, (DUNGEON_WIDTH - text_len) / 2, "Player is dead!");
    } else if (d->num_monsters_alive == 0) {
        int text_len = strlen("All monsters are dead!");
        mvprintw(0, (DUNGEON_WIDTH - text_len) / 2, "All monsters are dead!");
    }
    
    
    refresh();
    timeout(-1);  // Wait indefinitely for a keypress
    getch();
}

int get_input(Dungeon *d) {   
    while (1) {
        timeout(-1);
        int input = getch();
        int result;
        
        
        // Process directional keys
        switch (input) {
            case '7': // move up-left
            case 'y':
                result = handle_player_movement(d, d->pc.x - 1, d->pc.y - 1);
                break;
                
            case '8': // move up
            case 'k':
                result = handle_player_movement(d, d->pc.x, d->pc.y - 1);
                break;
                
            case '9': // move up-right
            case 'u':
                result = handle_player_movement(d, d->pc.x + 1, d->pc.y - 1);
                break;
                
            case '6': // move right
            case 'l':
                result = handle_player_movement(d, d->pc.x + 1, d->pc.y);
                break;
                
            case '3': // move down-right
            case 'n':
                result = handle_player_movement(d, d->pc.x + 1, d->pc.y + 1);
                break;
                
            case '2': // move down
            case 'j':
                result = handle_player_movement(d, d->pc.x, d->pc.y + 1);
                break;
                
            case '1': // move down-left
            case 'b':
                result = handle_player_movement(d, d->pc.x - 1, d->pc.y + 1);
                break;
                
            case '4': // move left
            case 'h':
                result = handle_player_movement(d, d->pc.x - 1, d->pc.y);
                break;
            
            case '5':
            case '.':
            case ' ': 
                render_top_bar("Player Skipped Their Turn", COLOR_WARNING_ID);
                return 1; // Skip turn
                
            case '<': // Up stairs
                result = handle_player_movement(d, -2, -2); // -2 for up stairs
                break;
                
            case '>': // Down stairs
                result = handle_player_movement(d, -3, -3); // -3 for down stairs
                break;
                
            case 'q': // quit
                destroy_ncurses();
                printf("Game terminated by user\n");
                exit(0);
                break;
            
            case 'm': // Monster list
                result = 0; // no movement, run loop again
                handle_monster_list(d);
                break;
                
            default:
                render_top_bar("Invalid Input", COLOR_ERROR_ID);
                continue; // Get input again
        }
        
        // Process the result from movement
        if (result == MOVEMENT_STAIRS) return MOVEMENT_STAIRS;
        if (result == 1) return 1;
    }
}

int handle_player_movement(Dungeon *d, int x, int y) {
    // mvprintw(0, 0, "Player at to (%d, %d) \t Player moving to: (%d, %d)", d->pc.x, d->pc.y, x, y);
    // refresh();

    char s[100];
    snprintf(s, sizeof(s), "Player at (%d, %d) \t Player moving to: (%d, %d)", d->pc.x, d->pc.y, x, y);
    render_top_bar(s, COLOR_DEFAULT_ID);

    int move_result = move_player(d, x, y);
    
    if (move_result == 0) { // invalid move
        render_top_bar("Invalid Player Movement, Try Again", COLOR_ERROR_ID);
        return 0;  // Return invalid movement code
    } else if (move_result == MOVEMENT_STAIRS) {
        render_top_bar("Player uses stairs", COLOR_STAIR_ID);
        return MOVEMENT_STAIRS; // Return stairs code
    }

    return 1; // Return successful movement
}
