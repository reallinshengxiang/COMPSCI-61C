/*
 * @Author: flwfdd
 * @Date: 2023-07-14 05:59:11
 * @LastEditTime: 2023-07-16 00:34:18
 * @Description: _(:з」∠)_
 */
#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"

int main(int argc, char* argv[]) {
  char* in_filename = NULL;
  char* out_filename = NULL;
  game_state_t* state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      in_filename = argv[i + 1];
      i++;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */

  // Read board from file, or create default board
  if (in_filename != NULL) {
    FILE* in_file = fopen(in_filename, "r");
    if(in_file == NULL){
      return -1;
    }
    fclose(in_file);
    state = load_board(in_filename);
    initialize_snakes(state);
  } else {
    state = create_default_state();
  }
  
  update_state(state,deterministic_food);

  // Write updated board to file or stdout
  if (out_filename != NULL) {
    FILE* out_file = fopen(out_filename, "w");
    if(out_file == NULL){
      return -1;
    }
    fclose(out_file);
    save_board(state, out_filename);
  } else {
    print_board(state, stdout);
  }

  // Free memory
  free_state(state);

  return 0;
}
