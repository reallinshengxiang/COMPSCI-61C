#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
// Necessary due to static functions in state.c
#include "state.c"

bool clear_unit_test_files() {
  int err_in = remove("unit-test-in.snk");
  int err_out = remove("unit-test-out.snk");
  int err_ref = remove("unit-test-ref.snk");
  return err_in == 0 && err_out == 0 && err_ref == 0;
}

bool test_create_default_state() {
  game_state_t* state = create_default_state();

  if (state == NULL) {
    printf("%s\n", "create_default_state is not implemented.");
    return false;
  }

  if (!assert_equals_unsigned_int("board width", DEFAULT_BOARD_WIDTH, 20)) {
    return false;
  }

  if (!assert_equals_unsigned_int("board height", DEFAULT_BOARD_HEIGHT, state->num_rows)) {
    return false;
  }

  // Test if the board is mutable
  // If it crashes here, your board is not mutable
  char original_char = get_board_at(state, 0, 0);
  set_board_at(state, 0, 0, '0');
  set_board_at(state, 0, 0, original_char);

  for (unsigned int row = 0; row < DEFAULT_BOARD_HEIGHT; row++) {
    for (unsigned int col = 0; col < DEFAULT_BOARD_WIDTH; col++) {
      if (row == 0 || col == 0 || row == DEFAULT_BOARD_HEIGHT - 1 || col == DEFAULT_BOARD_WIDTH - 1) {
        if (!assert_map_equals(state, row, col, '#')) {
          return false;
        }
      } else if (col == 9 && row == 2) {
        if (!assert_map_equals(state, row, col, '*')) {
          return false;
        }
      } else if (col == 2 && row == 2) {
        if (!assert_map_equals(state, row, col, 'd')) {
          return false;
        }
      } else if (col == 3 && row == 2) {
        if (!assert_map_equals(state, row, col, '>')) {
          return false;
        }
      } else if (col == 4 && row == 2) {
        if (!assert_map_equals(state, row, col, 'D')) {
          return false;
        }
      } else {
        if (!assert_map_equals(state, row, col, ' ')) {
          return false;
        }
      }
    }

    if (!(get_board_at(state, row, DEFAULT_BOARD_WIDTH) ==  '\0' ||
          (get_board_at(state, row, DEFAULT_BOARD_WIDTH) == '\n' && get_board_at(state, row, DEFAULT_BOARD_WIDTH + 1) == '\0'))) {
      printf("Warning: we think row %d of your board does not end in a null byte", row);
    }
  }

  if (!assert_equals_unsigned_int("number of snakes", 1, state->num_snakes)) {
    return false;
  }

  if (!assert_equals_unsigned_int("row of snake tail", 2, state->snakes->tail_row)) {
    return false;
  }
  if (!assert_equals_unsigned_int("col of snake tail", 2, state->snakes->tail_col)) {
    return false;
  }
  if (!assert_equals_unsigned_int("row of snake head", 2, state->snakes->head_row)) {
    return false;
  }
  if (!assert_equals_unsigned_int("col of snake head", 4, state->snakes->head_col)) {
    return false;
  }
  if (!assert_true("snake is alive", state->snakes->live)) {
    return false;
  }

  free_state(state);

  return true;
}

bool test_free_state() {
  game_state_t* state = create_default_state();
  free_state(state);

  printf("%s\n", "This test case only checks for leaks in Tasks 1 and 2. Make sure that no Valgrind errors are printed!");

  return true;
}

bool test_print_board_1() {
  clear_unit_test_files();

  char* expected =
    "####################\n"
    "#                  #\n"
    "# d>D    *         #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "####################\n";
  size_t file_size = strlen(expected);

  game_state_t* state = create_default_state();
  save_board(state, "unit-test-out.snk");

  // We hardcoded a filesize here since we know the board is 378 characters.
  // You shouldn't hardcode sizes in your own implementation.
  char actual[file_size + 1];

  FILE* f = fopen("unit-test-out.snk", "r");

  if (!assert_file_size("unit-test-out.snk", file_size)) {
    return false;
  }

  fread(actual, file_size, 1, f);
  fclose(f);
  actual[file_size] = '\0';

  if (strcmp(expected, actual) != 0) {
    printf("%s\n", "Your printed board doesn't match the expected output. See unit-test-out.snk for what you printed.");
    return false;
  }

  free_state(state);

  return true;
}

bool test_print_board_2() {
  clear_unit_test_files();

  char* expected =
    "####################\n"
    "#                  #\n"
    "# d>D    *         #\n"
    "####################\n";
  size_t file_size = strlen(expected);

  game_state_t* state = create_default_state();
  state->num_rows = 4;
  strncpy(state->board[3], state->board[0], DEFAULT_BOARD_WIDTH);
  save_board(state, "unit-test-out.snk");

  char actual[file_size + 1];

  FILE* f = fopen("unit-test-out.snk", "r");

  if (!assert_file_size("unit-test-out.snk", file_size)) {
    return false;
  }

  fread(actual, file_size, 1, f);
  fclose(f);
  actual[file_size] = '\0';

  if (strcmp(expected, actual) != 0) {
    printf("%s\n", "Your printed board doesn't match the expected output. See unit-test-out.snk for what you printed.");
    return false;
  }

  free_state(state);

  return true;
}

bool test_print_board() {
  if (!test_print_board_1()) {
    printf("%s\n", "test_print_board_1 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  if (!test_print_board_2()) {
    printf("%s\n", "test_print_board_2 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  return true;
}

bool test_next_square_board_1() {
  clear_unit_test_files();

  /*
  Board 1 (default):
  ####################
  #                  #
  # d>D    *         #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  // set up board
  game_state_t* actual_state = create_default_state();
  save_board(actual_state, "unit-test-out.snk");

  // the next square for the snake should be ' '
  if (!assert_equals_char("next_square on board 1", ' ', next_square(actual_state, 0))) {
    return false;
  }

  // check that board has not changed
  game_state_t* expected_state = create_default_state();
  if (!assert_state_equals(expected_state, actual_state)) {
    printf("%s\n", "Error: next_square should not modify board");
    return false;
  }

  free_state(actual_state);
  free_state(expected_state);

  return true;
}

bool test_next_square_board_2() {
  clear_unit_test_files();

  /*
  Board 2:
  ####################
  #                  #
  # d>D*   *         #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  game_state_t* state = create_default_state();

  // set up board
  set_board_at(state, 2, 5, '*');
  save_board(state, "unit-test-out.snk");

  // the next square for the snake should be '*'
  if (!assert_equals_char("next_square on board 2", '*', next_square(state, 0))) {
    return false;
  }

  free_state(state);

  return true;
}

bool test_next_square_board_3() {
  clear_unit_test_files();

  /*
  Board 3:
  ####################
  #                  #
  # d>Dx   *         #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  game_state_t* state = create_default_state();

  // set up board
  set_board_at(state, 2, 5, 'x');
  save_board(state, "unit-test-out.snk");

  // the next square for the snake should be 'x'
  if (!assert_equals_char("next_square on board 3", 'x', next_square(state, 0))) {
    return false;
  }

  free_state(state);

  return true;
}

bool test_next_square_board_4() {
  clear_unit_test_files();

  /*
  Board 4:
  ####################
  #   #              #
  # d>W    *         #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  game_state_t* state = create_default_state();

  // set up board
  set_board_at(state, 2, 4, 'W');
  set_board_at(state, 1, 4, '#');
  state->snakes->head_row = 2;
  state->snakes->head_col = 4;
  save_board(state, "unit-test-out.snk");

  // the next square for the snake should be '#'
  if (!assert_equals_char("next_square on board 4", '#', next_square(state, 0))) {
    return false;
  }

  free_state(state);

  return true;
}

bool test_next_square_board_5() {
  clear_unit_test_files();

  /*
  Board 5:
  ####################
  #                  #
  # d>v    *         #
  #   v              #
  #   S              #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  game_state_t* state = create_default_state();

  // set up board
  set_board_at(state, 2, 4, 'v');
  set_board_at(state, 3, 4, 'v');
  set_board_at(state, 4, 4, 'S');
  state->snakes->head_row = 4;
  state->snakes->head_col = 4;
  save_board(state, "unit-test-out.snk");

  // the next square for the snake should be ' '
  if (!assert_equals_char("next_square on board 5", ' ', next_square(state, 0))) {
    return false;
  }

  free_state(state);

  return true;
}

bool test_next_square_board_6() {
  clear_unit_test_files();

  /*
  Board 6:
  ####################
  #                  #
  # d>v    *         #
  #   v              #
  #   S              #
  #   #              #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  game_state_t* state = create_default_state();

  // set up board
  set_board_at(state, 2, 4, 'v');
  set_board_at(state, 3, 4, 'v');
  set_board_at(state, 4, 4, 'S');
  set_board_at(state, 5, 4, '#');
  state->snakes->head_row = 4;
  state->snakes->head_col = 4;
  save_board(state, "unit-test-out.snk");

  // the next square for the snake should be '#'
  if (!assert_equals_char("next_square on board 6", '#', next_square(state, 0))) {
    return false;
  }

  free_state(state);

  return true;
}

bool test_next_square() {
  if (!test_next_square_board_1()) {
    printf("%s\n", "test_next_square_board_1 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  if (!test_next_square_board_2()) {
    printf("%s\n", "test_next_square_board_2 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  if (!test_next_square_board_3()) {
    printf("%s\n", "test_next_square_board_3 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  if (!test_next_square_board_4()) {
    printf("%s\n", "test_next_square_board_4 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  if (!test_next_square_board_5()) {
    printf("%s\n", "test_next_square_board_5 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  if (!test_next_square_board_6()) {
    printf("%s\n", "test_next_square_board_6 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  return true;
}

bool test_update_head_board_1() {
  clear_unit_test_files();

  /*
  Board 1 (default):
  ####################            ####################
  #                  #            #                  #
  # d>D    *         #            # d>>D   *         #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 4, '>');
  set_board_at(expected, 2, 5, 'D');
  expected->snakes->head_row = 2;
  expected->snakes->head_col = 5;
  save_board(expected, "unit-test-ref.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  save_board(actual, "unit-test-in.snk");

  update_head(actual, 0);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_head_board_2() {
  clear_unit_test_files();

  /*
  Board 2:
  ####################            ####################
  #                  #            #   W              #
  # d>W    *         #            # d>^    *         #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 4, '^');
  set_board_at(expected, 1, 4, 'W');
  expected->snakes->head_row = 1;
  expected->snakes->head_col = 4;
  save_board(expected, "unit-test-ref.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 4, 'W');
  actual->snakes->head_row = 2;
  actual->snakes->head_col = 4;
  save_board(actual, "unit-test-in.snk");

  update_head(actual, 0);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_head_board_3() {
  clear_unit_test_files();

  /*
  Board 3:
  ####################            ####################
  #   A              #            #  A<              #
  # d>^    *         #            # d>^    *         #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 4, '^');
  set_board_at(expected, 1, 4, '<');
  set_board_at(expected, 1, 3, 'A');
  expected->snakes->head_row = 1;
  expected->snakes->head_col = 3;
  save_board(expected, "unit-test-ref.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 4, '^');
  set_board_at(actual, 1, 4, 'A');
  actual->snakes->head_row = 1;
  actual->snakes->head_col = 4;
  save_board(actual, "unit-test-in.snk");

  update_head(actual, 0);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_head() {
  if (!test_update_head_board_1()) {
    printf("%s\n", "test_update_head_board_1 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  if (!test_update_head_board_2()) {
    printf("%s\n", "test_update_head_board_2 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  if (!test_update_head_board_3()) {
    printf("%s\n", "test_update_head_board_3 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  return true;
}

bool test_update_tail_board_1() {
  clear_unit_test_files();

  /*
  Board 1:
  ####################            ####################
  #                  #            #                  #
  # d>>D   *         #            #  d>D   *         #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 2, ' ');
  set_board_at(expected, 2, 3, 'd');
  set_board_at(expected, 2, 4, '>');
  set_board_at(expected, 2, 5, 'D');
  expected->snakes->head_row = 2;
  expected->snakes->head_col = 5;
  expected->snakes->head_row = 2;
  expected->snakes->tail_col = 3;
  save_board(expected, "unit-test-ref.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 4, '>');
  set_board_at(actual, 2, 5, 'D');
  actual->snakes->head_row = 2;
  actual->snakes->head_col = 5;
  save_board(actual, "unit-test-in.snk");

  update_tail(actual, 0);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_tail_board_2() {
  clear_unit_test_files();

  /*
  Board 2:
  ####################            ####################
  #                  #            #                  #
  # dv     *         #            #   s    *         #
  #  v               #            #   v              #
  #  S               #            #   S              #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 2, ' ');
  set_board_at(expected, 2, 3, 's');
  set_board_at(expected, 2, 4, ' ');
  set_board_at(expected, 3, 3, 'v');
  set_board_at(expected, 4, 3, 'S');
  expected->snakes->head_row = 4;
  expected->snakes->head_col = 3;
  expected->snakes->tail_row = 2;
  expected->snakes->tail_col = 3;
  save_board(expected, "unit-test-ref.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 3, 'v');
  set_board_at(actual, 2, 4, ' ');
  set_board_at(actual, 3, 3, 'v');
  set_board_at(actual, 4, 3, 'S');
  actual->snakes->head_row = 4;
  actual->snakes->head_col = 3;
  save_board(actual, "unit-test-in.snk");

  update_tail(actual, 0);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_tail_board_3() {
  clear_unit_test_files();

  /*
  Board 3:
  ####################            ####################
  #                  #            #                  #
  # S<     *         #            # Sa     *         #
  #  w               #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();

  save_board(expected, "unit-test-ref.snk");
  set_board_at(expected, 2, 2, 'S');
  set_board_at(expected, 2, 3, 'a');
  set_board_at(expected, 2, 4, ' ');
  expected->snakes->head_row = 2;
  expected->snakes->head_col = 2;
  expected->snakes->tail_row = 2;
  expected->snakes->tail_col = 3;

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 2, 'S');
  set_board_at(actual, 2, 3, '<');
  set_board_at(actual, 2, 4, ' ');
  set_board_at(actual, 3, 3, 'w');
  actual->snakes->head_row = 2;
  actual->snakes->head_col = 2;
  actual->snakes->tail_row = 3;
  actual->snakes->tail_col = 3;
  save_board(actual, "unit-test-in.snk");

  update_tail(actual, 0);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_tail() {
  if (!test_update_tail_board_1()) {
    printf("%s\n", "test_update_tail_board_1 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  if (!test_update_tail_board_2()) {
    printf("%s\n", "test_update_tail_board_2 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  if (!test_update_tail_board_3()) {
    printf("%s\n", "test_update_tail_board_3 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  return true;
}

bool test_update_state_board_1() {
  clear_unit_test_files();

  /*
  Board 1 (default):
  ####################            ####################
  #                  #            #                  #
  # d>D    *         #            #  d>D   *         #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 2, ' ');
  set_board_at(expected, 2, 3, 'd');
  set_board_at(expected, 2, 4, '>');
  set_board_at(expected, 2, 5, 'D');
  expected->snakes->head_row = 2;
  expected->snakes->head_col = 5;
  expected->snakes->tail_row = 2;
  expected->snakes->tail_col = 3;
  save_board(expected, "unit-test-ref.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  save_board(actual, "unit-test-in.snk");

  update_state(actual, corner_food);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_state_board_2() {
  clear_unit_test_files();

  /*
  Board 2:
  ####################            ####################
  #                  #            #*                 #
  # d>D*   *         #            # d>>D   *         #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 4, '>');
  set_board_at(expected, 2, 5, 'D');
  set_board_at(expected, 1, 1, '*');
  expected->snakes->head_row = 2;
  expected->snakes->head_col = 5;
  save_board(expected, "unit-test-ref.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 5, '*');
  save_board(actual, "unit-test-in.snk");

  update_state(actual, corner_food);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_state_board_3() {
  clear_unit_test_files();

  /*
  Board 3:
  ####################            ####################
  # >W               #            # >x               #
  # w      *         #            # w      *         #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 2, 'w');
  set_board_at(expected, 2, 3, ' ');
  set_board_at(expected, 2, 4, ' ');
  set_board_at(expected, 1, 2, '>');
  set_board_at(expected, 1, 3, 'x');
  expected->snakes->head_row = 1;
  expected->snakes->head_col = 3;
  expected->snakes->live = false;
  save_board(expected, "unit-test-ref.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 2, 'w');
  set_board_at(actual, 2, 3, ' ');
  set_board_at(actual, 2, 4, ' ');
  set_board_at(actual, 1, 2, '>');
  set_board_at(actual, 1, 3, 'W');
  actual->snakes->head_row = 1;
  actual->snakes->head_col = 3;
  actual->snakes->tail_row = 2;
  actual->snakes->tail_col = 2;
  save_board(actual, "unit-test-in.snk");

  update_state(actual, 0);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_state_board_4() {
  clear_unit_test_files();

  /*
  Board 4:
  ####################            ####################
  # >v               #            # >v               #
  # wA     *         #            # wx     *         #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  # ---------> #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  #                  #            #                  #
  ####################            ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 2, 'w');
  set_board_at(expected, 2, 3, 'x');
  set_board_at(expected, 2, 4, ' ');
  set_board_at(expected, 1, 2, '>');
  set_board_at(expected, 1, 3, 'v');
  expected->snakes->head_row = 2;
  expected->snakes->head_col = 3;
  expected->snakes->live = false;
  save_board(expected, "unit-test-ref.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 2, 'w');
  set_board_at(actual, 1, 2, '>');
  set_board_at(actual, 1, 3, 'v');
  set_board_at(actual, 2, 3, 'A');
  set_board_at(actual, 2, 4, ' ');
  actual->snakes->head_row = 2;
  actual->snakes->head_col = 3;
  actual->snakes->tail_row = 2;
  actual->snakes->tail_col = 2;
  save_board(actual, "unit-test-in.snk");

  update_state(actual, corner_food);
  save_board(actual, "unit-test-out.snk");

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_update_state() {
  if (!test_update_state_board_1()) {
    printf("%s\n", "test_update_state_board_1 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  if (!test_update_state_board_2()) {
    printf("%s\n", "test_update_state_board_2 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  if (!test_update_state_board_3()) {
    printf("%s\n", "test_update_state_board_3 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  if (!test_update_state_board_4()) {
    printf("%s\n", "test_update_state_board_4 failed. Check unit-test-in.snk, unit-test-out.snk, and unit-test-ref.snk.");
    return false;
  }

  return true;
}

bool test_load_board_1() {
  clear_unit_test_files();

  /*
  Board 01-simple (default):
  ####################
  #                  #
  # d>D    *         #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */
  char* expected =
    "####################\n"
    "#                  #\n"
    "# d>D    *         #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "#                  #\n"
    "####################\n";
  if (!assert_load_equals("tests/01-simple-in.snk", expected)) {
    return false;
  }

  return true;
}

bool test_load_board_2() {
  clear_unit_test_files();

  /*
  Board 06-small (small):
  #####
  #   #
  # W #
  # ^ #
  # w #
  #####
  */
  char* expected =
    "#####\n"
    "#   #\n"
    "# W #\n"
    "# ^ #\n"
    "# w #\n"
    "#####\n";
  if (!assert_load_equals("tests/06-small-in.snk", expected)) {
    return false;
  }

  return true;
}

bool test_load_board_3() {
  clear_unit_test_files();

  /*
  Board 13-sus (non square):
  ##############
  #            #
  #  ########  #
  #  #      #  #####
  #  ########      #
  #                #
  #      #         #
  #                #
  #   ######   #####
  #   #    #   #
  #   #    #   #
  #####    #####

  */
  char* expected =
    "##############\n"
    "#            #\n"
    "#  ########  #\n"
    "#  #      #  #####\n"
    "#  ########      #\n"
    "#                #\n"
    "#      #         #\n"
    "#                #\n"
    "#   ######   #####\n"
    "#   #    #   #\n"
    "#   #    #   #\n"
    "#####    #####\n";
  if (!assert_load_equals("tests/13-sus-in.snk", expected)) {
    return false;
  }

  return true;
}

bool test_load_board() {
  if (!test_load_board_1()) {
    printf("%s\n", "test_load_board_1 failed. Check tests/01-simple-in.snk for a diagram of the board.");
    return false;
  }

  if (!test_load_board_2()) {
    printf("%s\n", "test_load_board_2 failed. Check tests/06-small-in.snk for a diagram of the board.");
    return false;
  }
  if (!test_load_board_3()) {
    printf("%s\n", "test_load_board_3 failed. Check tests/13-sus-in.snk for a diagram of the board.");
    return false;
  }

  return true;
}

bool test_find_head_board_1() {
  clear_unit_test_files();

  /*
  Board 1:
  ####################
  #                  #
  # d>v    *         #
  #   v  W           #
  #   v  ^           #
  #   >>>^           #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 4, 'v');
  set_board_at(expected, 3, 4, 'v');
  set_board_at(expected, 4, 4, 'v');
  set_board_at(expected, 5, 4, '>');
  set_board_at(expected, 5, 5, '>');
  set_board_at(expected, 5, 6, '>');
  set_board_at(expected, 5, 7, '^');
  set_board_at(expected, 4, 7, '^');
  set_board_at(expected, 3, 7, 'W');
  expected->snakes->head_row = 3;
  expected->snakes->head_col = 7;
  save_board(expected, "unit-test-out.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 4, 'v');
  set_board_at(actual, 3, 4, 'v');
  set_board_at(actual, 4, 4, 'v');
  set_board_at(actual, 5, 4, '>');
  set_board_at(actual, 5, 5, '>');
  set_board_at(actual, 5, 6, '>');
  set_board_at(actual, 5, 7, '^');
  set_board_at(actual, 4, 7, '^');
  set_board_at(actual, 3, 7, 'W');

  find_head(actual, 0);

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_find_head_board_2() {
  clear_unit_test_files();

  /*
  Board 2:
  ####################
  #                  #
  # d>Ds   *         #
  #    v             #
  #    S             #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 5, 's');
  set_board_at(expected, 3, 5, 'v');
  set_board_at(expected, 4, 5, 'S');
  expected->snakes = realloc(expected->snakes, sizeof(snake_t) * 2);
  snake_t* tmp = expected->snakes;
  tmp->tail_row = 2;
  tmp->tail_col = 2;
  tmp->head_row = 2;
  tmp->head_col = 4;
  tmp++;
  tmp->tail_row = 2;
  tmp->tail_col = 5;
  tmp->head_row = 4;
  tmp->head_col = 5;
  tmp->live = true;
  expected->num_snakes = 2;
  save_board(expected, "unit-test-out.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 5, 's');
  set_board_at(actual, 3, 5, 'v');
  set_board_at(actual, 4, 5, 'S');
  actual->snakes = realloc(actual->snakes, sizeof(snake_t) * 2);
  tmp = actual->snakes;
  tmp->tail_row = 2;
  tmp->tail_col = 2;
  tmp->head_row = 2;
  tmp->head_col = 4;
  tmp++;
  tmp->tail_row = 2;
  tmp->tail_col = 5;
  tmp->live = true;
  actual->num_snakes = 2;

  find_head(actual, 1);

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_find_head() {
  if (!test_find_head_board_1()) {
    printf("%s\n", "test_find_head_board_1 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  if (!test_find_head_board_2()) {
    printf("%s\n", "test_find_head_board_2 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  return true;
}

bool test_initialize_snakes_board_1() {
  clear_unit_test_files();

  /*
  Board 1 (default):
  ####################
  #                  #
  # d>D    *         #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  save_board(expected, "unit-test-out.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  free(actual->snakes);
  actual->num_snakes = 0;

  actual = initialize_snakes(actual);

  if (actual == NULL) {
    printf("%s\n", "initialize_snakes is not implemented.");
    return false;
  }

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_initialize_snakes_board_2() {
  clear_unit_test_files();

  /*
  Board 2:
  ####################
  #                  #
  # d>v    *         #
  #   v  W           #
  #   v  ^           #
  #   >>>^           #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  #                  #
  ####################
  */

  // set up expected board
  game_state_t* expected = create_default_state();
  set_board_at(expected, 2, 4, 'v');
  set_board_at(expected, 3, 4, 'v');
  set_board_at(expected, 4, 4, 'v');
  set_board_at(expected, 5, 4, '>');
  set_board_at(expected, 5, 5, '>');
  set_board_at(expected, 5, 6, '>');
  set_board_at(expected, 5, 7, '^');
  set_board_at(expected, 4, 7, '^');
  set_board_at(expected, 3, 7, 'W');
  expected->snakes->head_row = 3;
  expected->snakes->head_col = 7;
  save_board(expected, "unit-test-out.snk");

  // set up actual board
  game_state_t* actual = create_default_state();
  set_board_at(actual, 2, 4, 'v');
  set_board_at(actual, 3, 4, 'v');
  set_board_at(actual, 4, 4, 'v');
  set_board_at(actual, 5, 4, '>');
  set_board_at(actual, 5, 5, '>');
  set_board_at(actual, 5, 6, '>');
  set_board_at(actual, 5, 7, '^');
  set_board_at(actual, 4, 7, '^');
  set_board_at(actual, 3, 7, 'W');
  free(actual->snakes);
  actual->num_snakes = 0;

  actual = initialize_snakes(actual);

  if (actual == NULL) {
    printf("%s\n", "initialize_snakes is not implemented.");
    return false;
  }

  // check that actual board matches expected board
  if (!assert_state_equals(expected, actual)) {
    return false;
  }

  free_state(expected);
  free_state(actual);

  return true;
}

bool test_initialize_snakes() {
  if (!test_initialize_snakes_board_1()) {
    printf("%s\n", "test_initialize_snakes_board_1 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  if (!test_initialize_snakes_board_2()) {
    printf("%s\n", "test_initialize_snakes_board_2 failed. Check unit-test-out.snk for a diagram of the board.");
    return false;
  }

  return true;
}

/*
  Feel free to comment out tests.
*/
int main(int argc, char* argv[]) {
  bool MEMCHECK_MODE = false;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-m") == 0) {
      MEMCHECK_MODE = true;
      continue;
    }
    fprintf(stderr, "Usage: %s [-m]\n", argv[0]);
    return 1;
  }

  init_colors();

  printf("%s\n", "Reminder: These tests are not comprehensive, and passing them does not guarantee that your implementation is working.");

  if (MEMCHECK_MODE) {
    printf("\nTesting free_state...\n");
    if (!test_free_state()) {
      return 0;
    }
  } else {
    if (!test_and_print("create_default_state (Task 1)", test_create_default_state)) {
      return 0;
    }
    if (!test_and_print("print_board (Task 3)", test_print_board)) {
      return 0;
    }
    if (!test_and_print("next_square (Task 4)", test_next_square)) {
      return 0;
    }
    if (!test_and_print("update_head (Task 4)", test_update_head)) {
      return 0;
    }
    if (!test_and_print("update_tail (Task 4)", test_update_tail)) {
      return 0;
    }
    if (!test_and_print("update_state (Task 4)", test_update_state)) {
      return 0;
    }
    if (!test_and_print("load_board (Task 5)", test_load_board)) {
      return 0;
    }
    if (!test_and_print("find_head (Task 6)", test_find_head)) {
      return 0;
    }
    if (!test_and_print("initialize_snakes (Task 6)", test_initialize_snakes)) {
      return 0;
    }
  }
}
