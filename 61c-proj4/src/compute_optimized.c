#include <omp.h>
#include <x86intrin.h>

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
  #pragma omp parallel for collapse(1)
  for(int i=0;i<=max_num_i;i++) {
    for(int j=0;j<=b_matrix->cols-1;j++) {
      if(j+31<=b_matrix->cols-1) {
        __m256i numup1=_mm256_loadu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j));
        __m256i numdown1=_mm256_loadu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j));
        __m256i numup2=_mm256_loadu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j+8));
        __m256i numdown2=_mm256_loadu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j+8));
         __m256i numup3=_mm256_loadu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j+16));
        __m256i numdown3=_mm256_loadu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j+16));
        __m256i numup4=_mm256_loadu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j+24));
        __m256i numdown4=_mm256_loadu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j+24));
        _mm256_storeu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j),numup1);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j),numdown1);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j+8),numup2);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j+8),numdown2);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j+16),numup3);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j+16),numdown3);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j+24),numup4);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j+24),numdown4);
        j=j+31;
      }
      else if(j+15<=b_matrix->cols-1) {
        __m256i numup1=_mm256_loadu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j));
        __m256i numdown1=_mm256_loadu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j));
        __m256i numup2=_mm256_loadu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j+8));
        __m256i numdown2=_mm256_loadu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j+8));
        _mm256_storeu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j),numup1);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j),numdown1);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j+8),numup2);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j+8),numdown2);
        j=j+15;
      }
      else if(j+7<=b_matrix->cols-1) {
        __m256i numup=_mm256_loadu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j));
        __m256i numdown=_mm256_loadu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j));
        _mm256_storeu_si256((__m256i *)(b_matrix->data+(b_matrix->rows-1-i)*b_matrix->cols+j),numup);
        _mm256_storeu_si256((__m256i *)(b_matrix->data+i*b_matrix->cols+j),numdown);
        j=j+7;
      }
      else {
        int32_t tmp=b_matrix->data[i*b_matrix->cols+j];
        b_matrix->data[i*b_matrix->cols+j]=b_matrix->data[(b_matrix->rows-1-i)*b_matrix->cols+j];
        b_matrix->data[(b_matrix->rows-1-i)*b_matrix->cols+j]=tmp;
      }
    }
  }
  max_num_i=b_matrix->cols/2-1;
  if(max_num_i<0) max_num_i=0;
  #pragma omp parallel for collapse(2)
  for(int i=0;i<=max_num_i;i++) {
    for(int j=0;j<=b_matrix->rows-1;j++) {
      int32_t tmp=b_matrix->data[j*b_matrix->cols+i];
      b_matrix->data[j*b_matrix->cols+i]=b_matrix->data[j*b_matrix->cols+b_matrix->cols-i-1];
      b_matrix->data[j*b_matrix->cols+b_matrix->cols-i-1]=tmp;
    }
  }
  int block_size=32; 
  #pragma omp parallel for collapse(2) schedule(dynamic,1)
  for(int x_begin=0;x_begin<rows;x_begin=x_begin+block_size) {
    for(int y_begin=0;y_begin<cols;y_begin=y_begin+block_size) {
      int x_end=x_begin+block_size;
      if(x_end>rows) x_end=rows;
      int y_end=y_begin+block_size;
      if(y_end>cols) y_end=cols;
      for(int x=x_begin;x<x_end;x++) {
        for(int y=y_begin;y<y_end;y++) {
          __m256i sum_vec=_mm256_setzero_si256();
          for(int i=x;i<x+b_matrix->rows;i++) {
            for(int j=y;j<y+(b_matrix->cols/8)*8;j=j+8) {
              __m256i numa=_mm256_loadu_si256((__m256i *)(a_matrix->data+i*a_matrix->cols+j));
              __m256i numb=_mm256_loadu_si256((__m256i *)(b_matrix->data+(i-x)*b_matrix->cols+(j-y)));
              __m256i num=_mm256_mullo_epi32(numa,numb);
              sum_vec=_mm256_add_epi32(sum_vec,num);
            }
            for(int j=y+(b_matrix->cols/8)*8;j<y+b_matrix->cols;j++)  { 
            (*output_matrix)[0].data[x*cols+y]+=a_matrix->data[i*a_matrix->cols+j]*b_matrix->data[(i-x)*b_matrix->cols+(j-y)];
            }   
          }     
          int tmp[8];
          _mm256_storeu_si256((__m256i *)tmp,sum_vec);
          (*output_matrix)[0].data[x*cols+y]+=tmp[0]+tmp[1]+tmp[2]+tmp[3]+tmp[4]+tmp[5]+tmp[6]+tmp[7];
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
