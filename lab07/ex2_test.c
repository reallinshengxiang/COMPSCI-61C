#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "ex2.h"

/* -------------------------------Utilties, Do Not Modify------------------------------*/
double* gen_array(int n) {
    double* array = (double*)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) array[i] = drand48();
    return array;
}

int verify(double* x, double* y, void(*funct)(double *x, double *y, double *z)) {
    double *z_v_add = (double*) malloc(ARRAY_SIZE*sizeof(double));
    double *z_oracle = (double*) malloc(ARRAY_SIZE*sizeof(double));
    (*funct)(x, y, z_v_add);
    for(int i=0; i<ARRAY_SIZE; i++){
        z_oracle[i] = x[i] + y[i];
    }
    for(int i=0; i<ARRAY_SIZE; i++){
        if(z_oracle[i] != z_v_add[i])
            return 0;
    }
    return 1;
}

int main() {
  // Generate input vectors
  double *x = gen_array(ARRAY_SIZE);
  double *y = gen_array(ARRAY_SIZE);
  double serial_result = 0.0;
  double result;

  double naive = 0.0;
  double man_min = 100.0;
  double red_min = 100.0;

  double start_time, run_time;

  // calculate result serially
  for(int i=0; i<ARRAY_SIZE; i++)
    serial_result += x[i] * y[i];

  int num_threads = omp_get_max_threads();

  // Only run this once because it's too slow..
  omp_set_num_threads(1);
  start_time = omp_get_wtime();
  for(int j=0; j<REPEAT; j++) {
    result = dotp_naive(x, y, ARRAY_SIZE);
  }
  naive = omp_get_wtime() - start_time;
  printf("Naive: 1 thread took %f seconds\n",naive);

  // Test framework that sweeps the number of threads and times each run
  for (int i=1; i<=1; i++) {
    omp_set_num_threads(i);
    start_time = omp_get_wtime();
    for(int j=0; j<REPEAT; j++) {
      result = dotp_critical(x, y, ARRAY_SIZE);
    }
    run_time = omp_get_wtime() - start_time;

    // verify result is correct (within some threshold)
    if (fabs(serial_result - result) > 0.001) {
      printf("Critical does not match reference.\n");
      return -1;
    }

    printf("Critical: %d thread(s) took %f seconds\n",i,run_time);
  }

  for (int i=1; i<=num_threads; i++) {
    omp_set_num_threads(i);
    start_time = omp_get_wtime();
    for(int j=0; j<REPEAT; j++) {
      result = dotp_reduction(x, y, ARRAY_SIZE);
    }
    run_time = omp_get_wtime() - start_time;

    // verify result is correct (within some threshold)
    if (fabs(serial_result - result) > 0.001) {
      printf("Reduction optimized does not match reference.\n");
      return -1;
    }

    if (run_time < red_min) {
      red_min = run_time;
    }

    printf("Reduction Optimized: %d thread(s) took %f seconds\n",i,run_time);
  }

  if (red_min * 2.5 > naive) {
    printf("Fastest reduction optimized didn't achieve at least 2.5x speedup from naive.\n");
    return -1;
  }

  for (int i=1; i<=num_threads; i++) {
    omp_set_num_threads(i);
    start_time = omp_get_wtime();
    for(int j=0; j<REPEAT; j++) {
      result = dotp_manual_reduction(x, y, ARRAY_SIZE);
    }
    run_time = omp_get_wtime() - start_time;

    // verify result is correct (within some threshold)
    if (fabs(serial_result - result) > 0.001) {
      printf("Manual reduction does not match reference.\n");
      return -1;
    }

    if (run_time < man_min) {
      man_min = run_time;
    }

    printf("Manual reduction: %d thread(s) took %f seconds\n",i,run_time);
  }

  if (man_min * 2.5 > naive) {
    printf("Fastest Manual reduction didn't achieve at least 2.5x speedup from naive.\n");
    return -1;
  }

    printf("Congrats! All dotp tests passed\n");

  return 0;
}
