#ifndef IO_H
#define IO_H

#include <stdint.h>

typedef struct {
  uint32_t rows;
  uint32_t cols;
  int32_t *data;
} matrix_t;

typedef struct {
  char *path;
} task_t;

int read_tasks(char *input_file, int *num_tasks, task_t ***tasks);

char *get_a_matrix_path(task_t *task);
char *get_b_matrix_path(task_t *task);
char *get_output_matrix_path(task_t *task);

int read_matrix(char *path, matrix_t **matrix);
int write_matrix(char *path, matrix_t *matrix);

void print_matrix(matrix_t *matrix);

#endif
