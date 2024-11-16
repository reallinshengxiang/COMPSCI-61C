#include "coordinator.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Error: not enough arguments\n");
    printf("Usage: %s [path_to_task_list]\n", argv[0]);
    return -1;
  }

  // Read and parse task list file
  int num_tasks;
  task_t **tasks;
  if (read_tasks(argv[1], &num_tasks, &tasks)) {
    printf("Error reading task list from %s\n", argv[1]);
    return -1;
  }

  // Execute tasks
  for (int i = 0; i < num_tasks; i++) {
    if (execute_task(tasks[i])) {
      printf("Task %d failed\n", i);
      return -1;
    }
    free(tasks[i]->path);
    free(tasks[i]);
  }
  free(tasks);
}
