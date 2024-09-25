#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // TODO: Create space to store the string "hello"
  // You may use your solution from a previous exercise;
  char message[6];
  message[0] = 'h';
  message[1] = 'e';
  message[2] = 'l';
  message[3] = 'l';
  message[4] = 'o';
  message[5] = '\0';

  // Print out the value before we change message
  printf("Before copying: %s\n", message);

  // Creates another_string that contains a longer string
  char* long_message = "Here's a really long string";

  // TODO: Copy the string in long_message to message
  // IMPORTANT: This file is supposed to crash when you run it. You will be fixing this in another file.
  strcpy(message, long_message);

  // Print out the value after we change message
  printf("After copying: %s\n", message);

  return 0;
}
