
#ifndef EX2_H
#define EX2_H

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REPEAT 60
#define BUF_SIZE 8192

#define ARRAY_SIZE 5000000

double dotp_naive(double* x, double* y, int arr_size);
double dotp_critical(double* x, double* y, int arr_size);
double dotp_reduction(double* x, double* y, int arr_size);
double dotp_manual_reduction(double* x, double* y, int arr_size);

#endif
