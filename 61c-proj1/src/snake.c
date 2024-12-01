#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"

int main(int argc, char *argv[]) {
  bool io_stdin = false;
  char *in_filename = NULL;
  char *out_filename = NULL;
  game_state_t *state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      if (io_stdin) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      in_filename = argv[i + 1];
      i++;
      continue;
    } else if (strcmp(argv[i], "--stdin") == 0) {
      if (in_filename != NULL) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      io_stdin = true;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */

  // Read board from file, or create default board
  if (in_filename != NULL) {
    // TODO: Load the board from in_filename
    // TODO: If the file doesn't exist, return -1
    // TODO: Then call initialize_snakes on the state you made
    // TODO: close file pointer
    FILE*file = fopen(in_filename,"r");
    if(file==NULL) return -1;
    state=load_board(file);
    initialize_snakes(state);
    fclose(file);
    
  } else if (io_stdin) {
    // TODO: Load the board from stdin
    // TODO: Then call initialize_snakes on the state you made
    //FILE*file = fopen(in_filename,"r");
    //if(file==NULL) return -1;
    state=load_board(stdin);
    initialize_snakes(state);
    //fclose(file);
  } else {
     // TODO: Create default state
     state=create_default_state();
   
  }

  // TODO: Update state. Use the deterministic_food function
  // (already implemented in snake_utils.h) to add food.
  update_state(state,deterministic_food);
  // Write updated board to file or stdout
  if (out_filename != NULL) {
    // TODO: Save the board to out_filename
    FILE*file = fopen(out_filename,"w");
    if(out_filename==NULL) return -1;
    save_board(state,out_filename);
    fclose(file);
  } else {
    print_board(state,stdout);
    // TODO: Print the board to stdout
  }
  free_state(state);
  // TODO: Free the state
  
  return 0;
}
