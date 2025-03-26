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
static void print_monster_list(Dungeon *d);

int handle_monster_list(Dungeon *d){
    print_monster_list(d);
    return 0;
}

static void print_monster_list(Dungeon *d){
    clear();
    int i;
    int x = 0;
    int y = 0;
    char monster_symbol;

    for (i = 0; i < d->num_monsters; i++){
        monster_symbol= d->monsters[i].symbol;
        if (d->monsters[i].alive){
            mvprintw(y, x, "%c", monster_symbol);
            x++;
        }
        if (x >= DUNGEON_WIDTH - 1){
            x = 0;
            y++;
        }
    }
    refresh();
}