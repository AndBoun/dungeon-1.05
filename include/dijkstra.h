//
// Created by Andrew Boun on 2/25/25.
//

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <priority_queue.h>
#include <dungeon.h>



int create_non_tunneling_map(Dungeon *d, int pc_x, int pc_y);
int create_tunneling_map(Dungeon *d, int pc_x, int pc_y);

#endif