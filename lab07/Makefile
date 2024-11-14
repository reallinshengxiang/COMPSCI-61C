CC=gcc
CFLAGS_SIMD=-std=c99 -Wall -O0 -c
CFLAGS_OMP=-std=gnu99 -fopenmp -c -g
LDFLAGS_OMP=-fopenmp

ex1: ex1_test.o ex1.o
	$(CC) $^ -o $@

ex1_test.o: ex1_test.c
	$(CC) $(CFLAGS_SIMD) $< -o $@

ex1.o: ex1.c ex1.h
	$(CC) $(CFLAGS_SIMD) $< -o $@

openmp_example: openmp_example.o
	$(CC) $(LDFLAGS_OMP) $^ -o $@

openmp_example.o: openmp_example.c
	$(CC) $(CFLAGS_OMP) $< -o $@

ex2: ex2_test.o ex2.o
	$(CC) $(LDFLAGS_OMP) $^ -o $@

ex2_test.o: ex2_test.c
	$(CC) $(CFLAGS_OMP) $< -o $@

ex2.o: ex2.c ex2.h
	$(CC) $(CFLAGS_OMP) $< -o $@

clean:
	rm -f ex1
	rm -f ex2
	rm -f openmp_example
	rm -f *.o
