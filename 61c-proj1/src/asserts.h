#ifndef _ASSERTS_H
#define _ASSERTS_H

#include "state.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern const unsigned int DEFAULT_BOARD_WIDTH;
extern const unsigned int DEFAULT_BOARD_HEIGHT;

bool assert_true(char *msg, bool actual);
bool assert_false(char *msg, bool actual);
bool assert_equals_bool(char *msg, bool expected, bool actual);
bool assert_equals_char(char *msg, char expected, char actual);
bool assert_equals_int(char *msg, int expected, int actual);
bool assert_equals_unsigned_int(char *msg, unsigned int expected, unsigned int actual);
bool assert_load_equals(FILE *fp, char *expected);
bool assert_map_equals(game_state_t *state, unsigned int row, unsigned int col, char expected);
bool assert_state_equals(game_state_t *expected, game_state_t *actual);
bool assert_file_size(char *fd, size_t expected_file_size);
void init_colors();
bool test_and_print(char *label, bool (*run_test)());

#endif
