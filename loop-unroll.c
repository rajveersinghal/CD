#include <stdio.h>

#define SIZE 16 // Array size
#define UNROLL_FACTOR 4 // Unroll factor

int main() {
    int arr[SIZE];
    
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1;
    }
    // Loop unrolling (unroll by a factor of 4)
    int i = 0;
    for (; i <= SIZE - UNROLL_FACTOR; i += UNROLL_FACTOR) {
        arr[i]     = arr[i]     + 5;
        arr[i + 1] = arr[i + 1] + 5;
        arr[i + 2] = arr[i + 2] + 5;
        arr[i + 3] = arr[i + 3] + 5;
    }
    // Handle remaining elements if SIZE is not divisible by UNROLL_FACTOR
    for (; i < SIZE; i++) {
        arr[i] = arr[i] + 5;
    }
    // Print the updated array
    for (int i = 0; i < SIZE; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    return 0;
}
