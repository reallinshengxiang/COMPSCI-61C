#include <stdio.h>

void add_one(int input) {
  input += 1;
}

// TODO: create a pointer to input
void add_one_pointer(int* input) {
  // TODO: add one to the integer that input points to
  *input += 1;
}

int main() {
  // Assign x (an integer) to 5
  int x = 5;
  // Call add_one on x
  add_one(x);

  // This line should print 5
  // Why doesn't this work?
  printf("add_one: %d\n", x);
  // Let's try using add_one_pointer
  int *x_point=NULL;
  x_point=&x;
  // TODO: use add_one_pointer to increment x
  // Hint: compare the type of x with the type of the argument
  //       for add_one_pointer
  add_one_pointer(x_point);

  // This line should print 6
  printf("add_one_pointer: %d\n", x);

  return 0;
}
