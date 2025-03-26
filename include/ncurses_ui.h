//
// Created by Andrew Boun on 3/24/25.
//

#ifndef NCURSES_UI_H
#define NCURSES_UI_H

#include <dungeon.h>
#include <ncurses.h>

void init_ncurses();
void destroy_ncurses();

void render_top_bar(const char *message, int color_id);

void render_grid(Dungeon *d);
void render_game_over(Dungeon *d);

int get_input(Dungeon *d);
int handle_monster_list(Dungeon *d);

#endif
