#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ex1.h"

/* ***DON'T MODIFY THIS FILE! You should only need to modify simd.c!*** */

int main(int argc, char* argv[]) {
    printf("Let's generate a randomized array.\n");
    int vals[NUM_ELEMS];
    long long int reference;
    long long int simd;
    long long int simdu;
    for (unsigned int i = 0; i < NUM_ELEMS; i++) vals[i] = rand() % 256;
    int success = 0;

    printf("Starting randomized sum.\n");
    clock_t start = clock();
    reference = sum(vals);
    clock_t end = clock();
    printf("Sum: %lld\n\n", reference);
    clock_t reft = end - start;

    printf("Starting randomized unrolled sum.\n");
    printf("Sum: %lld\n\n", sum_unrolled(vals));

    printf("Starting randomized SIMD sum.\n");
    start = clock();
    simd = sum_simd(vals);
    end = clock();
    printf("Sum: %lld\n\n", simd);
    clock_t simdt = end - start;

    if (simd != reference) {
        printf("Test Failed! SIMD sum %lld doesn't match reference sum %lld!\n\n", simd, reference);
        success = 1;
    }

    if (reft <= simdt * 2) {
        printf("Test Failed! SIMD sum provided less than 2X speedup.\n\n");
        success = 1;
    }

    printf("Starting randomized SIMD unrolled sum.\n");
    start = clock();
    simdu = sum_simd_unrolled(vals);
    end = clock();
    printf("Sum: %lld\n\n", simdu);
    clock_t simdut = end - start;

    if (simdu != reference) {
        printf("Test Failed! SIMD unrolled sum %lld doesn't match reference sum %lld!\n\n", simdu, reference);
        success = 1;
    }

    if (simdt <= simdut) {
        printf("Test Failed! SIMD unrolled function provided no speedup.\n\n");
        success = 1;
    }

    if (!success) {
        printf("All tests Passed! Correct values were produced, and speedups were achieved!\n\n");
        return 0;
    } else {
        return 1;
    }
}
