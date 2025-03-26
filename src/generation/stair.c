//
// Created by Andrew Boun on 2/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dungeon.h>

bool place_stair(Dungeon *d, int x, int y, char stair) {
    if (x > 0 && x <= PLACABLE_WIDTH && y > 0 && y < PLACABLE_HEIGHT) {
        d->grid[y][x].type = stair;
        d->grid[y][x].hardness = MIN_HARDNESS;
        return true;
    }
    return false;
}

int generate_random_stair(Dungeon *d, char stair, int idx){
    int x, y;
    do {
        x = (rand() % PLACABLE_WIDTH) + 1;
        y = (rand() % PLACABLE_HEIGHT) + 1;
    } while (d->grid[y][x].type == ROCK);

    d->grid[y][x].type =  stair;
    d->grid[y][x].hardness = MIN_HARDNESS;

    if (stair == UP_STAIRS) {
        d->up_stairs[idx].x = x;
        d->up_stairs[idx].y = y;
    } else {
        d->down_stairs[idx].x = x;
        d->down_stairs[idx].y = y;
    }
    
    return 1;
}

int is_up_stair(Dungeon *d, int x, int y){
    for (int i = 0; i < d->num_up_stairs; i++){
        if (d->up_stairs[i].x == x && d->up_stairs[i].y == y){
            return 1;
        }
    }
    return 0;
}

int is_down_stair(Dungeon *d, int x, int y){
    for (int i = 0; i < d->num_down_stairs; i++){
        if (d->down_stairs[i].x == x && d->down_stairs[i].y == y){
            return 1;
        }
    }
    return 0;
}