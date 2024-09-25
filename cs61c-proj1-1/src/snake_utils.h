#ifndef _SNK_SNAKE_UTILS_H
#define _SNK_SNAKE_UTILS_H

#include <stdint.h>
#include "state.h"

/* Define key presses for interactive mode. */
#define KEY_MOVEUP 0x77
#define KEY_MOVERIGHT 0x64
#define KEY_MOVEDOWN 0x73
#define KEY_MOVELEFT 0x61
#define KEY_QUIT 0x71

/* A simple deterministic random function. Look up LFSR to learn more! */
unsigned int det_rand(unsigned int* state);

/* Deterministically generates food on the board. */
int deterministic_food(game_state_t* state);

/* Generates food in the top-left corner of the board. */
int corner_food(game_state_t* state);

/* Changes the direction of the player-controlled snake. */
void redirect_snake(game_state_t* state, char input_direction);

/* Randomly causes the chosen snake to turn left or right. */
void random_turn(game_state_t* state, int snum);

#endif
