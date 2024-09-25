#include <stdlib.h>

void jedi(int *arr, int size);
void ben(int *arr, int size);
void jero(int *arr, int size);

void jedi(int *arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = i * 10;
    }
}

void ben(int *arr, int size) {
    int *ptr = NULL;
    *ptr = 10;
    jero(arr, size);
}

void jero(int *arr, int size) {
    int *new_arr = (int *)malloc(size * sizeof(int));
    if (new_arr == NULL) {
        return;
    }
    for (int i = 0; i < size; ++i) {
        new_arr[i] = arr[i];
    }
    jedi(new_arr, size);
    free(new_arr);
}

int main() {
    int arr[10];
    ben(arr, 10);
    return 0;
}
