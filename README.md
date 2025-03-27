# How To Run The Project:

 Create a `.rlg327` folder in your home directory:
```bash
mkdir ~/.rlg327
```

Build the project:
```bash
make
```

## Running configurations
Run the project with flags (or no flags):
```bash
./dungeon --load --save --nummon <number>
```

> # Dungeon File
> Whenever a save or load occurs, it uses the file called `dungeon`. This doesn't have any extension and cannot have one for loading to work.

# Main Files:
```Plain
include/
└── ncurses_ui.h
src/
└── io
	├── monster_list.c
	└── ncurses_ui.c
```
# About the Project:

## Constraints:
- All files and inputs are assumed to be valid
- There is no limit on the number of monsters, but too high of an input will cause an infinite loop, as there's no space to place monsters
- The monster list will only show a maximum of 10 monsters at once

## Additional Notes
- If the user enter an invalid input, the game pauses and until they make a correct move. To skip a turn, the user must specifically pass their turn.
- When exiting the monster list, the user can alternatively press `m` again to exit back to the game much faster.

## How the Project Runs:
After loading and generating the Dungeon in `main.c`, the program calls `init_ncurses()` in `ncurses_ui.c` to create the UI. Then `start_gameplay()` in `dungeon.c`. This method will call `get_input()` which takes player input then calls the respective methods. When traveling using stairs, the coordinate values `(-2, -2)` indicate going upstairs, and `(-3, -3)` indicate going downstairs. When the movement function explicitly returns `-2` or `MOVEMENT_STAIRS`, this means the player correct traveled through stairs, and the dungeon needs to be regenerate.

To regenerate when the player uses a stair, first the program calls `start_gameplay()` then `get_input()`, then `handle_player_movement()` in `ncurses_ui.c`, which calls `move_player()` in `player_movement.c`. This function will return `-2`, which makes `handle_player_movement()`, which also makes `get_input()` return `-2


`. Now the `start_gameplay()` method terminates, then back in `main.c`,  the dungeon will be reset with `reset_dungeon()` and gameplay will start again.

# Example Gameplay:

## Video:
https://github.com/user-attachments/assets/b69686fc-56af-42d4-81d8-cee3cc53a63c

## GIF:
![1 05_gameplay](https://github.com/user-attachments/assets/51d2e122-3796-4f4a-8748-89d20c2a4353)

