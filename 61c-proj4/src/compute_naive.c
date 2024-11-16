#include "compute.h"

// Computes the convolution of two matrices
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  // TODO: convolve matrix a and matrix b, and store the resulting matrix in
  // output_matrix
  
  int32_t rows=a_matrix->rows-b_matrix->rows+1,cols=a_matrix->cols-b_matrix->cols+1;
  *output_matrix=(matrix_t *)malloc(1*sizeof(matrix_t));
  if (*output_matrix==NULL) return -1;
  (*output_matrix)[0].rows=rows;
  (*output_matrix)[0].cols=cols;
  (*output_matrix)[0].data=(int32_t *)calloc(cols*rows,sizeof(int32_t));
  if((*output_matrix)[0].data==NULL) {
    free((*output_matrix)[0].data);
    free(*output_matrix);
    return -1;
  }
  int max_num_i=b_matrix->rows/2-1;
  if(max_num_i<0) max_num_i=0;
  for(int i=0;i<=max_num_i;i++) {
    for(int j=0;j<=b_matrix->cols-1;j++) {
      int32_t tmp=b_matrix->data[i*b_matrix->cols+j];
      b_matrix->data[i*b_matrix->cols+j]=b_matrix->data[(b_matrix->rows-1-i)*b_matrix->cols+j];
      b_matrix->data[(b_matrix->rows-1-i)*b_matrix->cols+j]=tmp;
    }
  }
  max_num_i=b_matrix->cols/2-1;
  if(max_num_i<0) max_num_i=0;
  for(int i=0;i<=max_num_i;i++) {
    for(int j=0;j<=b_matrix->rows-1;j++) {
      int32_t tmp=b_matrix->data[j*b_matrix->cols+i];
      b_matrix->data[j*b_matrix->cols+i]=b_matrix->data[j*b_matrix->cols+b_matrix->cols-i-1];
      b_matrix->data[j*b_matrix->cols+b_matrix->cols-i-1]=tmp;
    }
  }
  for(int x=0;x<=rows-1;x++) {
    for(int y=0;y<=cols-1;y++) {
      for(int i=x;i<=x+b_matrix->rows-1;i++) {
        for(int j=y;j<=y+b_matrix->cols-1;j++){
          (*output_matrix)[0].data[x*cols+y]+=a_matrix->data[i*a_matrix->cols+j]*b_matrix->data[(i-x)*b_matrix->cols+j-y];
        }        
      }
    }  
  }
  return 0;
}
  
// Executes a task
int execute_task(task_t *task) {
  matrix_t *a_matrix, *b_matrix, *output_matrix;

  char *a_matrix_path = get_a_matrix_path(task);
  if (read_matrix(a_matrix_path, &a_matrix)) {
    printf("Error reading matrix from %s\n", a_matrix_path);
    return -1;
  }
  free(a_matrix_path);

  char *b_matrix_path = get_b_matrix_path(task);
  if (read_matrix(b_matrix_path, &b_matrix)) {
    printf("Error reading matrix from %s\n", b_matrix_path);
    return -1;
  }
  free(b_matrix_path);

  if (convolve(a_matrix, b_matrix, &output_matrix)) {
    printf("convolve returned a non-zero integer\n");
    return -1;
  }

  char *output_matrix_path = get_output_matrix_path(task);
  if (write_matrix(output_matrix_path, output_matrix)) {
    printf("Error writing matrix to %s\n", output_matrix_path);
    return -1;
  }
  free(output_matrix_path);

  free(a_matrix->data);
  free(b_matrix->data);
  free(output_matrix->data);
  free(a_matrix);
  free(b_matrix);
  free(output_matrix);
  return 0;
}
