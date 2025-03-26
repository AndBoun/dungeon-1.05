//
// Created by Andrew Boun on 2/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include <dungeon.h>
#include <load_save.h>
#include <dijkstra.h>
#include <ncurses_ui.h>


int main(int argc, char *argv[]) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec ^ getpid());

    Dungeon d;

    int load_flag = 0;
    int save_flag = 0;
    int num_monsters = DEFAULT_NUM_MONSTERS;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--load") == 0) {
            load_flag = 1;
        } else if (strcmp(argv[i], "--save") == 0) {
            save_flag = 1;
        } else if (strcmp(argv[i], "--nummon") == 0) {
            num_monsters = atoi(argv[++i]);
        }
    }

    init_dungeon(&d);
    d.num_monsters = num_monsters;

    if (load_flag) {
        load(&d);
        // print_grid(&d);
    } else {
        generate_random_dungeon(&d);
        // print_grid(&d);
    }

    if (save_flag) {
        save(&d);
        // printf("Dungeon saved.\n");
    }

    // print_grid(&d);
    // reset_dungeon(&d);
    // print_grid(&d);

    // print_grid(&d);
    // d.pc.x = 38, d.pc.y = 13;
    // handle_player_movement(&d, -2, -2);
    // // move_player(&d, -2, -2);
    // printf("%c", d.grid[13][38].type);

    init_ncurses();
    
    // start_gameplay(&d);

    while (start_gameplay(&d) == -2){
        reset_dungeon(&d);
    }

    destroy_dungeon(&d);

    destroy_ncurses();

    return 0;
}