#define _POSIX_C_SOURCE 199506L

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include "snake_utils.h"
#include "state.h"

// This code uses some pretty bad hacks, and should not be used as a "good" reference.

struct timespec game_interval = {1, 0L};
game_state_t* state = NULL;
pthread_mutex_t state_mutex;

// Adapted from https://stackoverflow.com/a/912796
int get_raw_char() {
  if (!isatty(STDIN_FILENO)) {
    return getchar();
  }

  unsigned char buf = 0;
  struct termios old = {0};
  if (tcgetattr(STDIN_FILENO, &old) < 0) {
    perror("Error getting terminal attributes");
  }
  tcflag_t old_lflag = old.c_lflag;
  old.c_lflag &= (tcflag_t) ~ICANON;
  old.c_lflag &= (tcflag_t) ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(STDIN_FILENO, TCSANOW, &old) < 0) {
    perror("Error disabling terminal canonical mode");
  }
  if (read(STDIN_FILENO, &buf, 1) < 0) {
    perror("Error reading char from terminal");
  }
  old.c_lflag = old_lflag;
  if (tcsetattr(STDIN_FILENO, TCSADRAIN, &old) < 0) {
    perror("Error re-enabling terminal canonical mode");
  }
  return (int) (unsigned char) buf;
}

void print_fullscreen_board(game_state_t* state) {
  fprintf(stdout, "\033[2J\033[H");
  print_board(state, stdout);
}

void* game_loop(void* _) {
  unsigned int timestep = 0;
  print_fullscreen_board(state);

  while (1) {
    nanosleep(&game_interval, NULL);

    pthread_mutex_lock(&state_mutex);
    int live_snakes = 0;
    // non-player controlled snakes randomly turn every 6 steps
    for (int j = 0; j < state->num_snakes; j++) {
      if (state->snakes[j].live) {
        live_snakes += 1;
        if (j >= 1 && timestep % 6 == 0) {
          random_turn(state, j);
        }
      }
    }
    update_state(state, deterministic_food);
    pthread_mutex_unlock(&state_mutex);

    print_fullscreen_board(state);
    timestep += 1;

    if (live_snakes == 0) {
      break;
    }
  }

  return NULL;
}

void input_loop() {
  while (1) {
    char key = (char) get_raw_char();
    pthread_mutex_lock(&state_mutex);
    if (key == '[') {
      if (game_interval.tv_nsec >= 900000000L) {
        game_interval.tv_sec++;
        game_interval.tv_nsec = 0L;
      } else {
        game_interval.tv_nsec += 100000000L;
      }
    } else if (key == ']') {
      if (game_interval.tv_nsec == 0L) {
        game_interval.tv_sec--;
        game_interval.tv_nsec = 900000000L;
      } else if (game_interval.tv_sec > 0 || game_interval.tv_nsec > 100000000L) {
        game_interval.tv_nsec -= 100000000L;
      }
    } else {
      redirect_snake(state, key);
    }
    pthread_mutex_unlock(&state_mutex);
    print_fullscreen_board(state);
  }
}

int main(int argc, char* argv[]) {
  char* in_filename = NULL;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      in_filename = argv[i + 1];
      i++;
      continue;
    }
    if (strcmp(argv[i], "-d") == 0 && i < argc - 1) {
      double delay = strtod(argv[i + 1], NULL);
      if (delay == 0.0 && errno != 0) {
        perror("Error parsing delay");
      }
      game_interval.tv_sec = (time_t) (unsigned int) delay;
      game_interval.tv_nsec = (long) (delay * 1000000000) % 1000000000L;
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename] [-d delay]\n", argv[0]);
    return 1;
  }

  // Read board from file, or create default board
  if (in_filename != NULL) {
    state = load_board(in_filename);
    state = initialize_snakes(state);
  } else {
    state = create_default_state();
  }

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, game_loop, NULL);
  input_loop();
  pthread_cancel(thread_id);

  return 0;
}
