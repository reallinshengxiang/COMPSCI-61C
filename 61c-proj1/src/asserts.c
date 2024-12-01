#define _POSIX_C_SOURCE 200809L
#include "asserts.h"

#include "state.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const unsigned int DEFAULT_BOARD_WIDTH = 20;
const unsigned int DEFAULT_BOARD_HEIGHT = 18;

/*
  Returns true if actual is equal to true
  Othewise, will return false with this message:
  "Assertion error: expected (msg) to be true, but got false"
*/
bool assert_true(char *msg, bool actual) {
  if (!actual) {
    printf("Assertion error: expected %s to be true, but got false\n", msg);
    return false;
  } else {
    return true;
  }
}

/*
  Returns true if actual is equal to false
  Othewise, will return false with this message:
  "Assertion error: expected (msg) to be false, but got true"
*/
bool assert_false(char *msg, bool actual) {
  if (actual) {
    printf("Assertion error: expected %s to be false, but got true\n", msg);
    return false;
  } else {
    return true;
  }
}

/*
  Returns true if expected is equal to actual
  Otherwise, will return false with this message:
  "Assertion error: expected (msg) to be (expected) but got (actual)"
*/
bool assert_equals_char(char *msg, char expected, char actual) {
  if (expected != actual) {
    printf("Assertion error: expected %s to be %c but got %c\n", msg, expected, actual);
    return false;
  } else {
    return true;
  }
}

/*
  Returns true if expected is equal to actual
  Otherwise, will return false with this message:
  "Assertion error: expected (msg) to be (expected) but got (actual)"
*/
bool assert_equals_int(char *msg, int expected, int actual) {
  if (expected != actual) {
    printf("Assertion error: expected %s to be %d but got %d\n", msg, expected, actual);
    return false;
  } else {
    return true;
  }
}

/*
  Returns true if expected is equal to actual
  Otherwise, will return false with this message:
  "Assertion error: expected (msg) to be (expected) but got (actual)"
*/
bool assert_equals_unsigned_int(char *msg, unsigned int expected, unsigned int actual) {
  if (expected != actual) {
    printf("Assertion error: expected %s to be %d but got %d\n", msg, expected, actual);
    return false;
  } else {
    return true;
  }
}

/*
  Returns true if expected is equal to actual
  Otherwise, will return false with this message:
  "Assertion error: expected (msg) to be (expected) but got (actual)"
*/
bool assert_equals_bool(char *msg, bool expected, bool actual) {
  if (expected != actual) {
    printf("Assertion error: expected %s to be %s but got %s\n", msg, expected ? "true" : "false",
           actual ? "true" : "false");
    return false;
  } else {
    return true;
  }
}

bool assert_load_equals(FILE *fp, char *expected) {
  game_state_t *state = load_board(fp);

  if (state == NULL) {
    printf("%s\n", "load_board is not implemented, skipping...");
    return false;
  }

  unsigned int expectedLines = 0;
  unsigned int expectedLen = (unsigned)strlen(expected);
  for (unsigned int i = 0; i < expectedLen; i++) {
    if (expected[i] == '\n') {
      expectedLines++;
    }
  }

  if (!assert_equals_unsigned_int("board height", expectedLines, state->num_rows)) {
    return false;
  }

  save_board(state, "unit-test-out.snk");

  FILE *f = fopen("unit-test-out.snk", "r");
  fseek(f, 0, SEEK_END);
  unsigned int f_len = (unsigned int)ftell(f);
  if (expectedLen != f_len) {
    printf("%s\n", "Your loaded board doesn't match the expected output. See unit-test-out.snk for what you loaded.");
    fclose(f);
    return false;
  }
  rewind(f);
  char *actual = malloc(sizeof(char) * (f_len + 1));
  if (actual == NULL) {
    printf("%s\n", "Failed to allocate memory for file.");
    return false;
  }
  fread(actual, f_len, 1, f);
  actual[f_len] = '\0';
  fclose(f);

  if (strcmp(expected, actual) != 0) {
    printf("%s\n", "Your loaded board doesn't match the expected output. See unit-test-out.snk for what you loaded.");
    return false;
  }
  free(actual);

  free_state(state);

  return true;
}

bool assert_map_equals(game_state_t *state, unsigned int row, unsigned int col, char expected) {
  char actual = get_board_at(state, row, col);
  if (expected != actual) {
    printf("Assertion error: at (row %d, col %d), expected %c but got %c\n", row, col, expected, actual);
    return false;
  } else {
    return true;
  }
}

// Note: This function only works for states created from create_default_state.
bool assert_state_equals(game_state_t *expected, game_state_t *actual) {
  // Check that width and height are equal
  if (!assert_equals_unsigned_int("board height", expected->num_rows, actual->num_rows)) {
    return false;
  }
  if (!assert_equals_unsigned_int("board width", (unsigned int)strlen(expected->board[0]),
                                  (unsigned int)strlen(actual->board[0]))) {
    return false;
  }

  // Check that boards are equal
  for (unsigned int row = 0; row < DEFAULT_BOARD_HEIGHT; row++) {
    for (unsigned int col = 0; col < DEFAULT_BOARD_WIDTH; col++) {
      char expected_char = get_board_at(expected, row, col);
      char actual_char = get_board_at(actual, row, col);
      if (expected_char != actual_char) {
        printf("Assertion error: at (row %d, col %d), expected %c but got %c\n", row, col, expected_char, actual_char);
        return false;
      }
    }
  }

  // Check that num_snakes are equal
  if (!assert_equals_unsigned_int("number of snakes", expected->num_snakes, actual->num_snakes)) {
    return false;
  }

  // Check that snake coordinates and live are equal
  // Note: This code checks all snakes, so there may be errors if state->snakes was not allocated correctly.
  for (unsigned int i = 0; i < expected->num_snakes; i++) {
    if (!assert_equals_unsigned_int("row of snake tail", expected->snakes[i].tail_row, actual->snakes[i].tail_row)) {
      return false;
    }
    if (!assert_equals_unsigned_int("col of snake tail", expected->snakes[i].tail_col, actual->snakes[i].tail_col)) {
      return false;
    }
    if (!assert_equals_unsigned_int("row of snake head", expected->snakes[i].head_row, actual->snakes[i].head_row)) {
      return false;
    }
    if (!assert_equals_unsigned_int("col of snake head", expected->snakes[i].head_col, actual->snakes[i].head_col)) {
      return false;
    }
    if (!assert_equals_bool("snake is alive", expected->snakes[i].live, actual->snakes[i].live)) {
      return false;
    }
  }

  return true;
}

bool assert_file_size(char *file, size_t expected_file_size) {
  struct stat buffer;
  if (stat(file, &buffer)) {
    printf("Assertion error: unable to check file size.\n");
    return false;
  }
  if (expected_file_size != (size_t)buffer.st_size) {
    printf("Assertion error: expected file size to be %d but got %d.\n", (int)expected_file_size, (int)buffer.st_size);
    return false;
  }
  return true;
}

char *COLOR_GREEN = "";
char *COLOR_RESET = "";

void init_colors() {
  if (!isatty(STDOUT_FILENO)) {
    return;
  }

  if (getenv("NO_COLOR") != NULL) {
    return;
  }

  char *term = getenv("TERM");
  if (term == NULL || strstr(term, "xterm") == NULL) {
    return;
  }

  COLOR_GREEN = "\033[0;32m";
  COLOR_RESET = "\033[0m";
}

bool test_and_print(char *label, bool (*run_test)()) {
  printf("\nTesting %s...\n", label);
  bool result = run_test();
  if (result) {
    printf("%sAll %s tests passed!%s\n", COLOR_GREEN, label, COLOR_RESET);
  } else {
    printf("Not all %s tests passed.\n", label);
  }
  return result;
}
