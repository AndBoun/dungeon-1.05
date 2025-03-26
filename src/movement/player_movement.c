//
// Created by Andrew Boun on 3/4/25.
//

#include <stdio.h>
#include <stdlib.h>

#include <character.h>
#include <dungeon.h>
#include <dijkstra.h>

#include <ncurses_ui.h>

#include <unistd.h>

int move_player(Dungeon *d, int x, int y){

    // Check stairs
    if (
        (x == -2 && y == -2 && is_up_stair(d, d->pc.x, d->pc.y)) || 
        (x == -3 && y == -3 && is_down_stair(d, d->pc.x, d->pc.y))
    ) {
        return -2; // Up or down stairs
    }

    // Check if the move is invalid (same position, rock, or non-zero hardness)
    if (
        (x == d->pc.x && y == d->pc.y) || 
        (d->grid[y][x].type == ROCK)   || 
        (d->grid[y][x].hardness > 0)   ||
        (x < 0 || x >= DUNGEON_WIDTH - 1)  || 
        (y < 0 || y >= DUNGEON_HEIGHT - 1)
    ) {
        // printf("Player made an invalid move to (%d, %d)\n", x, y);
        return 0;
    }

    // Check if the new cell is occupied, and kill the occupant
    if (
        (d->grid[y][x].type >= '0' && d->grid[y][x].type <= '9') || 
        (d->grid[y][x].type >= 'A' && d->grid[y][x].type <= 'F')
    ){
        // printf("Player killed a monster: %c\n", d->grid[y][x].type);
        kill_monster(d, x, y);
    }

    // printf("Player moved from (%d, %d) to (%d, %d)\n", d->pc.x, d->pc.y, x, y);

    d->grid[d->pc.y][d->pc.x].type = d->pc.curr_cell; // return the cell to its original type
    d->pc.curr_cell = d->grid[y][x].type; // update the current cell type
    d->grid[y][x].type = PLAYER; // Move the player on the grid array

    d->pc.x = x;
    d->pc.y = y;

    create_non_tunneling_map(d, d->pc.x, d->pc.y);
    create_tunneling_map(d, d->pc.x, d->pc.y);

    return 1;
}