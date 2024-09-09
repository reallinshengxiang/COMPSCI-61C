#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    char* name;
} Student;

Student* create_student_1(int id) {
  // TODO: allocate memory to store a Student struct
  Student* student_ptr = malloc(sizeof(Student));

  // TODO: set student_ptr's id to the id argument of this function
  student_ptr->id = id;

  return student_ptr;
}

void create_student_2(Student** student_double_ptr, int id) {
  // TODO: fill the space that student_double_ptr points to with the address of
  //       some memory large enough for a Student struct
  // Hint: you may need to use the dereference operator here
  *student_double_ptr = malloc(sizeof(Student));

  // TODO: set student_double_ptr's id to the id argument of this function
  (*student_double_ptr)->id = id;
}


int main() {
  // TODO: use create_student_1 to create a pointer to a Student struct
  //       where the student has id of 5
  Student* student1_ptr = create_student_1(5);

  // TODO: print the id of the student that student1_ptr points to
  printf("Student 1's ID: %d\n", student1_ptr->id);

  // TODO: create a pointer that can point to a Student struct
  //       do not allocate any memory
  Student* student2_ptr;

  // TODO: use create_student_2 to populate the student2_ptr
  //       where the student has id of 6
  // Hint: compare the type of student2_ptr with the type of
  //       the argument for create_student_2
  create_student_2(&student2_ptr, 6);

  // TODO: print the id of the student that student2_ptr points to
  printf("Student 2's ID: %d\n", student2_ptr->id);

  // Free everything allocated with `malloc`
  free(student1_ptr);
  free(student2_ptr);

  return 0;
}
